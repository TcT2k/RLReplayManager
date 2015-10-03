/*
** RLReplayManager
**
** Copyright (C) 2015 Tobias Taschner <github@tc84.de>
**
** Licensed under GPL v3 or later
*/
#ifndef Replay_H
#define Replay_H

#include <wx/string.h>
#include <wx/vector.h>
#include <wx/any.h>
#include <wx/stream.h>
#include <wx/sharedptr.h>
#include <map>

class ReplayProperties : public std::map<wxString, wxAny>
{
public:
	typedef wxVector<ReplayProperties> List;

	ReplayProperties();

	ReplayProperties(wxInputStream& istr);

	void Load(wxInputStream& istr);

protected:
	static wxString ReadString(wxInputStream& istr);
private:
};

class Replay: public ReplayProperties
{
public:
	typedef wxSharedPtr<Replay> Ptr;

	Replay(const wxString& filename);

	void Load(const wxString& filename);

	const wxString& GetDescription() const
		{ return m_description; }

	const wxDateTime& GetDate() const
		{ return m_date; }

	const wxDateTime& GetLength() const
		{ return m_length; }

	wxString GetExportFileName();

	wxDateTime ConvertFrames(wxUint32 frames) const;

	void Export(const wxString& filename);

private:
	wxString m_filename;
	wxString m_description;
	wxDateTime m_date;
	wxDateTime m_length;
	wxDouble m_fps;

	wxString GetExportBaseName();
};

#endif // Replay_H