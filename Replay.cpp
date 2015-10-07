/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/
#include <wx/wxprec.h>

#include "Replay.h"

#include <wx/log.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

//
// ReplayProperties
//

ReplayProperties::ReplayProperties()
{
}

ReplayProperties::ReplayProperties(wxInputStream& istr)
{
	Load(istr);
}

void ReplayProperties::Load(wxInputStream& istr)
{
	while (istr.IsOk())
	{
		wxString propertyName = ReadString(istr);
		if (propertyName.IsSameAs("None"))
			break;

		wxString propertyType = ReadString(istr);
		wxUint64 propertySize;
		istr.Read(&propertySize, sizeof(propertySize));

		if (propertyType.IsSameAs("IntProperty"))
		{
			wxUint32 value;
			istr.Read(&value, sizeof(value));
			(*this)[propertyName] = value;
		}
		else if (propertyType.IsSameAs("StrProperty")
			|| propertyType.IsSameAs("NameProperty"))
		{
			(*this)[propertyName] = ReadString(istr);
		}
		else if (propertyType.IsSameAs("FloatProperty"))
		{
			wxFloat32 value;
			istr.Read(&value, sizeof(value));

			(*this)[propertyName] = value;
		}
		else if (propertyType.IsSameAs("ArrayProperty"))
		{
			wxUint32 entryCount;
			istr.Read(&entryCount, sizeof(entryCount));

			List arrayValues;
			for (wxUint32 i = 0; i < entryCount; i++)
			{
				ReplayProperties subProps;
				subProps.Load(istr);

				arrayValues.push_back(subProps);
			}

			(*this)[propertyName] = arrayValues;
		}
		else
		{
			wxLogError(_("Unknown property type: %s"), propertyType);
			break;
		}

	}
}

wxString ReplayProperties::ReadString(wxInputStream& istr)
{
	wxInt32 len;
	istr.Read(&len, sizeof(len));
	if (len == 0)
		return wxString();

	if (len < 0)
	{
		// Unicode string
		len = abs(len);
		wxWCharBuffer chBuf(len - 1);

		istr.Read(chBuf.data(), (len - 1) * 2);
		istr.SeekI(2, wxFromCurrent); // Skip trailing zero

		return chBuf;
	}
	else
	{
		wxCharBuffer chBuf(len - 1);

		istr.Read(chBuf.data(), len - 1);
		istr.SeekI(1, wxFromCurrent); // Skip trailing zero

		return chBuf;
	}
}

//
// Replay
//
std::map<wxString, wxString> Replay::ms_mapNames;

Replay::Replay(const wxString& filename)
{
	Load(filename);
}

void Replay::Load(const wxString& filename)
{
	m_filename = filename;

	wxFileName fn(filename);

	wxFileInputStream istr(filename);

	wxUint32 val1;
	istr.Read(&val1, sizeof(val1));

	wxUint32 val2;
	istr.Read(&val2, sizeof(val2));

	wxUint32 val3;
	istr.Read(&val3, sizeof(val3));

	wxUint32 val4;
	istr.Read(&val4, sizeof(val4));

	wxString gameId = ReadString(istr);
	if (!gameId.IsSameAs("TAGame.Replay_Soccar_TA"))
	{
		wxLogError(_("Unsupported game type: %s"), gameId);
		return;
	}

	ReplayProperties::Load(istr);

	m_description = (*this)["Id"].As<wxString>();

	m_date.ParseFormat((*this)["Date"].As<wxString>(), wxS("%Y-%m-%d:%H-%M"));

	m_fps = (*this)["RecordFPS"].As<wxFloat32>();
	m_length = ConvertFrames((*this)["NumFrames"].As<wxUint32>());
}

wxDateTime Replay::ConvertFrames(wxUint32 frames) const
{
	wxDouble seconds = frames / m_fps;
	wxUint32 minutes = seconds / 60;
	wxUint32 hours = 0;
	if (minutes > 60)
	{
		hours = minutes / 60;
		minutes %= 60;
	}

	return wxDateTime(hours, minutes, (int)seconds % 60, 0);
}

wxString Replay::GetExportBaseName()
{
	wxString str = "Replay_";

	str += (*this)["PlayerName"].As<wxString>();

	str += "_";

	int teamSize = (*this)["TeamSize"].As<wxUint32>();

	if (teamSize == 1)
		str += "Duel";
	else
		str += wxString::Format("%don%d", teamSize, teamSize);

	str += "_";

	str += m_date.Format("%Y-%m-%d_%H-%M");

	return str;
}


wxString Replay::GetExportFileName()
{
	return GetExportBaseName() + ".zip";
}

void Replay::Export(const wxString& filename)
{
	wxFileInputStream istr(m_filename);
	wxFileOutputStream ostr(filename);
	wxZipOutputStream zipstr(ostr);

	wxString replayPath = "My Games/Rocket League/TAGame/Demos/";

	zipstr.PutNextEntry(replayPath + GetExportBaseName() + ".replay", m_date);
	istr.Read(zipstr);
	
	zipstr.CloseEntry();
}

wxString Replay::GetMapDisplayName()
{
	wxString mapId = (*this)["MapName"].As<wxString>();

	if (ms_mapNames.empty())
	{
		wxFileName cfgName(wxStandardPaths::Get().GetResourcesDir(), "RLReplayManager.cfg");
		wxFileConfig cfg(wxEmptyString, wxEmptyString, wxEmptyString, cfgName.GetFullPath());
		cfg.SetPath("/MapNames");
		wxString entry;
		long entryIndex = 0;
		bool moreEntries = cfg.GetFirstEntry(entry, entryIndex);
		while (moreEntries)
		{
			ms_mapNames[entry] = cfg.Read(entry);
			moreEntries = cfg.GetNextEntry(entry, entryIndex);
		}
	}

	auto mapDesc = ms_mapNames.find(mapId.Lower());
	if (mapDesc != ms_mapNames.end())
		return mapDesc->second;
	else
		return mapId;
}
