#pragma once

enum DDL_QUERY_INDEX
{
	DELETE_CASCADE_ON = 0,
	CREATE_SCENARIO_LIST_TABLE,
	DROP_SCENARIO_LIST_TABLE,
	CREATE_NOTE_INFORMATION_TABLE,
	DROP_NOTE_INFORMATION_TABLE,
	CREATE_PAD_OPTION_TABLE,
	DROP_PAD_OPTION_TABLE,
	CREATE_TIME_LINE_TABLE,
	DROP_TIME_LINE_TABLE
};

enum DML_QUERY_INDEX
{
	UPDATE_AUTOINCREMENT_RESET_SCENARIO_LIST = 0,
	UPDATE_AUTOINCREMENT_RESET_NOTE_INFORMATION,
	SELECT_ALL_SCENARIO_LIST_TABLE,
	SELECT_ALL_NOTE_INFORMATION_TABLE,
	SELECT_IN_SEQ_NOTE_INFORMATION_TABLE,
	SELECT_ALL_PAD_OPTION_TABLE,
	INSERT_PAD_OPTION_TABLE,
	UPDATE_PAD_OPTION_TABLE,
	INSERT_SCENARIO_LIST_TABLE,
	UPDATE_SCENARIO_LIST_TABLE,
	DELETE_SCENARIO_LIST_TABLE,
	INSERT_NOTE_INFORMATION_TABLE,
	UPDATE_NOTE_INFORMATION_TABLE,
	DELETE_NOTE_INFORMATION_TABLE,
	SELECT_ONE_NOTE_INFORMATION_TABLE,
	UPDATE_SCENARIO_LIST_TABLE_IN_NOTCNT,
	UPDATE_SCENARIO_LIST_TABLE_IN_SCETITLE,
	UPDATE_NOTE_INFORMATION_TABLE_IN_NOTCONTENT,
	UPDATE_NOTE_INFORMATION_TABLE_IN_NOTLOCK,
	SELECT_IN_SCESEQ_SCENARIO_LIST_IN_NOTSEQ,
	SELECT_IN_NOTCNT_SCENARIO_LIST_IN_SCESEQ,
	SELECT_IN_SCETITLE_SCENARIO_LIST_IN_SCESEQ,
	UPDATE_NOTE_INFORMATION_TABLE_IN_SCESEQ,
	SELECT_ALL_TIME_LINE_TABLE_IN_SCESEQ,
	INSERT_TIME_LINE_TABLE,
	DELETE_TIME_LINE_TABLE,
	UPDATE_TIME_LINE_TABLE_IN_TIMEIDX,
	UPDATE_NOTE_INFORMATION_TABLE_IN_SETTIMELINE,
	SELECT_COUNT_TIME_LINE_TABLE_IN_SCESEQ,
	SELECT_IN_TIMEIDX_TIME_LINE_TABLE_IN_NOTSEQ,
	UPDATE_TIME_LINE_TABLE_IN_TIMEIDX_IN_SCESEQ,
};

static const char* DefinedDDLQuerys[] = { 
									// on delete cascade 
									"PRAGMA foreign_keys=ON",
									// create scenario list table
									"CREATE TABLE ScenarioList(sceSEQ INTEGER PRIMARY KEY AUTOINCREMENT, \
																sceTITLE TEXT NOT NULL, \
																notCNT INTEGER DEFAULT 0)",	
									// drop scenario list table
									"DROP TABLE ScenarioList",	
									// create note information table
									"CREATE TABLE NoteInformation(notSEQ INTEGER PRIMARY KEY AUTOINCREMENT, \
																	notCONTENT TEXT, \
																	setTIMELINE INTEGER DEFAULT 0, \
																	notLOCK INTEGER DEFAULT 0, \
																	sceSEQ INTEGER, FOREIGN KEY (sceSEQ) REFERENCES ScenarioList(sceSEQ) ON DELETE CASCADE)",
									// drop note information table
									"DROP TABLE NoteInformation",
									// create pad option table
									"CREATE TABLE PadOption(test1 INTEGER DEFAULT 0, \
															test2 INTEGER DEFAULT 0)",
									// drop pad option table
									"DROP TABLE PadOption",
									// create timeline table
									"CREATE TABLE Timeline(timeIDX INTEGER DEFAULT 0, \
															notSEQ INTEGER, \
															sceSEQ INTEGER, \
															FOREIGN KEY (notSEQ) REFERENCES NoteInformation(notSEQ) ON DELETE CASCADE, \
															FOREIGN KEY (sceSEQ) REFERENCES ScenarioList(sceSEQ) ON DELETE CASCADE)",
									// drop timeline table
									"DROP TABLE Timeline",

								  };

static const char* DefinedDMLQuerys[] = {
									// update scenario list primary key sequence reset
									"UPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name = 'ScenarioList'",
									// update note information primary key sequence reset
									"UPDATE SQLITE_SEQUENCE SET seq = 0 WHERE name = 'NoteInformation'",
									// select all scenario list table
									"SELECT * FROM ScenarioList ORDER BY sceSEQ ASC",
									// select all note information table
									"SELECT * FROM NoteInformation",
									// select note information table in sceSEQ
									"SELECT * FROM NoteInformation WHERE sceSEQ = ? ORDER BY notSEQ ASC",
									// select pad option table
									"SELECT * FROM PadOption",
									// insert pad option table
									"INSERT INTO PadOption (test1, test2) VALUES (?, ?)",
									// update pad option table
									"UPDATE PadOption SET test1 = ?, test2 = ?",
									// insert scenario list table
									"INSERT INTO ScenarioList (sceTITLE, notCNT) VALUES (?, 0)",
									// update scenario list table
									"UPDATE ScenarioList SET sceTITLE = ?, notCNT = ? WHERE sceSEQ = ?",
									// delete scenario list table
									"DELETE FROM ScenarioList WHERE sceSEQ = ?",
									// insert note information table
									"INSERT INTO NoteInformation (notCONTENT, setTIMELINE, notLOCK, sceSEQ) VALUES (?, ?, ?, ?)",
									// update note information table
									"UPDATE NoteInformation SET notCONTENT = ?, setTIMELINE = ?, notLOCK = ? WHERE notSEQ = ?",
									// delete note information table
									"DELETE FROM NoteInformation WHERE notSEQ = ?",
									// select one note information table
									"SELECT * FROM NoteInformation WHERE notSEQ = ?",
									// update scenario list table in notCNT
									"UPDATE ScenarioList SET notCNT = ? WHERE sceSEQ = ?",
									// update scenario list table in sceTITLE
									"UPDATE ScenarioList SET sceTITLE = ? WHERE sceSEQ = ?",
									// update note information table in notCONTENT
									"UPDATE NoteInformation SET notCONTENT = ? WHERE notSEQ = ?",
									// update not information table in notLOCK
									"UPDATE NoteInformation SET notLOCK = ? WHERE notSEQ = ?",
									// select sceSEQ, notCNT scenario list in notCNT
									"SELECT sceSEQ, notCNT FROM ScenarioList WHERE sceSEQ = (SELECT sceSEQ FROM NoteInformation WHERE notSEQ = ?)",
									// select notCNT scenario list in sceSEQ
									"SELECT notCNT FROM ScenarioList WHERE sceSEQ = ?",
									// select sceSEQ scenario list in sceTITLE
									"SELECT sceSEQ FROM ScenarioList WHERE sceTITLE = ?",
									// update note information table in sceSEQ
									"UPDATE NoteInformation SET sceSEQ = ? WHERE notSEQ = ?",
									// select one time line table in sceSEQ
									"SELECT * FROM Timeline WHERE sceSEQ = ? ORDER BY timeIDX ASC",
									// insert time line table
									"INSERT INTO Timeline (timeIDX, notSEQ, sceSEQ) VALUES (?, ?, ?)",
									// delete time line table
									"DELETE FROM Timeline WHERE notSEQ = ?",
									// update time line in timeIDX
									"UPDATE Timeline SET timeIDX = ? WHERE timeIDX = ? AND notSEQ = ?",
									// update note information in setTIMELINE
									"UPDATE NoteInformation SET setTIMELINE = ? WHERE notSEQ = ?",
									// select count time line table
									"SELECT COUNT(*) FROM Timeline WHERE sceSEQ = ?",
									// select timeIDX time line table in notSEQ
									"SELECT timeIDX FROM Timeline WHERE notSEQ = ?",
									// update time line in timeIDX sceSEQ
									"UPDATE Timeline SET timeIDX = ? WHERE timeIDX = ? AND sceSEQ = ?",
								   };
