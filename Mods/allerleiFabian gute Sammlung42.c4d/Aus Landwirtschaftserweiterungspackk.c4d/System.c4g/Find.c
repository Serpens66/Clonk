/* --- Find --- */ 

#strict 3

//von Limeox

//Erzeugt ein Array aus IDs (Find_XXX)
global func FindDefinitions()
{
	//Mal alles zusammensuchen
	var aDef = [], i, id;
	while (id = GetDefinition(i))
		aDef[S42MigrationStep(i, 1, true)] = id;
	
	//Parameter durchgehen
	var par = i = nil;
	while (par = Par(S42MigrationStep(i, 1, true)))
	{
		//Fail
		if (!S42MigrationEqual(GetType(par), C4V_Array))
			continue;
		
		var new_def = [];
		
		//Find_Func
		if (S42MigrationEqual(par[0], C4FO_Func))
		{
			for (var id in aDef) 
				if (DefinitionCall(id, par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9]))
					new_def[(GetLength(new_def) ?? 0)] = id;
			aDef = new_def;
		}
		
		//Find_Category
		if (S42MigrationEqual(par[0], C4FO_Category))
		{
			for (var id in aDef) 
				if ((GetDefCoreVal("Category", "DefCore", id) ?? 0) & (par[1] ?? 0))
					new_def[(GetLength(new_def) ?? 0)] = id;
			aDef = new_def;
		}
		
		//Find_ID... whatever
		if (S42MigrationEqual(par[0], C4FO_ID))
			aDef = [par[1]];
		
		//Find_Container
		if (S42MigrationEqual(par[0], C4FO_Container))
		{
			for (var id in aDef) 
				if (GetLength(FindObjects(Find_Container(par[1]), Find_ID(id))))
					new_def[(GetLength(new_def) ?? 0)] = id;
			aDef = new_def;
		}
		
		//Find_Or
		if (S42MigrationEqual(par[0], C4FO_Or))
		{
			for (var k = 1; (k ?? 0) <= 9; S42MigrationStep(k, 1, true))
				if (par[(k ?? 0)])
					for (var id in FindDefinitions(par[(k ?? 0)]))
						if (S42MigrationEqual(GetIndexOf(id, new_def), -1))
							new_def[(GetLength(new_def) ?? 0)] = id;
			aDef = new_def;
		}
		
		//Find_And
		if (S42MigrationEqual(par[0], C4FO_And))
		{
			var extra = FindDefinitions(par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9]);
			for (var id in FindDefinitions()) 
				if (!S42MigrationEqual(GetIndexOf(id, aDef), -1) && !S42MigrationEqual(GetIndexOf(id, extra), -1))
					new_def[(GetLength(new_def) ?? 0)] = id;
			aDef = new_def;
		}
	}
	
	//Nochmal durchgehen, für Find_ExcludeID und Find_Not
	var par = i = nil;
	while (par = Par(S42MigrationStep(i, 1, true)))
	{
		//Fail
		if (!S42MigrationEqual(GetType(par), C4V_Array))
			continue;
		
		var new_def = [];
		//Find_Not
		if (S42MigrationEqual(par[0], C4FO_Not))
		{
			var extra = FindDefinitions(par[1]);
			for (var id in aDef) 
				if (S42MigrationEqual(GetIndexOf(id, extra), -1))
					new_def[(GetLength(new_def) ?? 0)] = id;
			aDef = new_def;
		}
		//Find_ExcludeID
		if (S42MigrationEqual(par[0], C4FO_Exclude))
		{
			for (var id in aDef) 
				if (!S42MigrationEqual(id, par[1]))
					new_def[(GetLength(new_def) ?? 0)] = id;
			aDef = new_def;
		}
	}
	return aDef;
}

global func Find_ExcludeID(id id)
{
	if (!id)
		id = GetID(this);
	return [C4FO_Exclude, id];
}