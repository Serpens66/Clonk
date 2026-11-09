/* --- Find --- */ 

#strict 2

//von Limeox

//Erzeugt ein Array aus IDs (Find_XXX)
global func FindDefinitions()
{
	//Mal alles zusammensuchen
	var aDef = [], i, id;
	while (id = GetDefinition(i))
		aDef[i++] = id;
	
	//Parameter durchgehen
	var par = i = 0;
	while (par = Par(i++))
	{
		//Fail
		if (GetType(par) != C4V_Array)
			continue;
		
		var new_def = [];
		
		//Find_Func
		if (par[0] == C4FO_Func)
		{
			for (var id in aDef) 
				if (DefinitionCall(id, par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9]))
					new_def[GetLength(new_def)] = id;
			aDef = new_def;
		}
		
		//Find_Category
		if (par[0] == C4FO_Category)
		{
			for (var id in aDef) 
				if (GetDefCoreVal("Category", "DefCore", id) & par[1])
					new_def[GetLength(new_def)] = id;
			aDef = new_def;
		}
		
		//Find_ID... whatever
		if (par[0] == C4FO_ID)
			aDef = [par[1]];
		
		//Find_Container
		if (par[0] == C4FO_Container)
		{
			for (var id in aDef) 
				if (GetLength(FindObjects(Find_Container(par[1]), Find_ID(id))))
					new_def[GetLength(new_def)] = id;
			aDef = new_def;
		}
		
		//Find_Or
		if (par[0] == C4FO_Or)
		{
			for (var k = 1; k <= 9; k++)
				if (par[k])
					for (var id in FindDefinitions(par[k])) 
						if (GetIndexOf(id, new_def) == -1)
							new_def[GetLength(new_def)] = id;
			aDef = new_def;
		}
		
		//Find_And
		if (par[0] == C4FO_And)
		{
			var extra = FindDefinitions(par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9]);
			for (var id in FindDefinitions()) 
				if (GetIndexOf(id, aDef) != -1 && GetIndexOf(id, extra) != -1)
					new_def[GetLength(new_def)] = id;
			aDef = new_def;
		}
	}
	
	//Nochmal durchgehen, für Find_ExcludeID und Find_Not
	var par = i = 0;
	while (par = Par(i++))
	{
		//Fail
		if (GetType(par) != C4V_Array)
			continue;
		
		var new_def = [];
		//Find_Not
		if (par[0] == C4FO_Not)
		{
			var extra = FindDefinitions(par[1]);
			for (var id in aDef) 
				if (GetIndexOf(id, extra) == -1)
					new_def[GetLength(new_def)] = id;
			aDef = new_def;
		}
		//Find_ExcludeID
		if (par[0] == C4FO_Exclude)
		{
			for (var id in aDef) 
				if (id != par[1])
					new_def[GetLength(new_def)] = id;
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