/* Serialisierung der Objekte */

#strict 3

global func GetObjects()
{
	// Objekte serialisieren
	var data = SerializeObjects(nil,nil,LandscapeWidth(), LandscapeHeight(), Find_And(Find_Not(Find_Func("IsHUD")),Find_Not(Find_Func("IsLight"))));
	
	// Aufbereiten
	return data;
}

global func LogObjects() {
	var objects = GetObjects();
	
	var reference = [[],[]];
	var obj, data;
	
	var i = nil;
	// Alle Objekte
	for (data in objects)
	{
		// Daten in data.
		// [0] ID
		// [1] X-Pos
		// [2] Y-Pos
		// [3] Besitzer
		// [4] Extra (weitere Aufrufe)
		// [5] Noch mehr Extra (???)
	
		// Sonderbehandlung (z.B. PlaceSpawnpoint)
		if (S42MigrationEqual(data[0], -1))
		{
			Log("%s;",Format(data[4], data[1], data[2]));
		}
		// Ansonsten normal
		else
		{
			// CreateObject-String formatieren.
			var str = Format("CreateObject(%i,%d,%d,%d);",data[0], data[1], data[2], data[3]);

			var v = nil;
			
			// falls noch was mit dem Objekt geschieht muss dieses Objekt gemerkt werden
			for (var extra in data[4])
			{
			
				// vor den Logs also ausgeben...
				if(!v) {
					v = true;
					Log("var obj%d = %s",i,str);
				}
				
				if (S42MigrationEqual(GetType(extra), C4V_Array)) {
					reference[0][(GetLength(reference[0]) ?? 0)] = [i, extra];
				} else {
					Log("obj%d->%s;", i, extra);
				}
			}
			for (var j in data[5])
			{
				// auch hier... aber nicht doppelt
				if(!v) {
					v = true;
					Log("var obj%d = %s",i,str);
				}
				reference[1][(j ?? 0)] = i;
			}
			
			// Variablenname benutzt, hochzählen
			if(v) {
				S42MigrationStep(i, 1);
			// ansonsten das CreateObject ausgeben
			} else {
				Log(str);
			}

		}
	}
	for (data in reference[0]) {
		// Hier kommt es normalerweise zu einem BUG!!! :(
		Log("obj%d->%s;", data[0], Format(data[1][0], reference[1][(data[1][1] ?? 0)], reference[1][(data[1][2] ?? 0)], reference[1][(data[1][3] ?? 0)], reference[1][(data[1][4] ?? 0)], reference[1][(data[1][5] ?? 0)]));
	}
}

global func SerializeObjects(iX, int iY, int iW, int iH, exclusions)
{
	// Arraykompabilität
	iY ??= 0;
	iW ??= 0;
	iH ??= 0;
	if (!iW || !iH) {
		iY = iX[1]; iW = iX[2]; iH = iX[3];
		iX = iX[0];
	}
	
	var exp = Find_And(
		Find_InRect(iX, iY, iW, iH),
		Find_Not(Find_Func("NoSerialize")),
		Find_Not(Find_Category(C4D_Goal)),
		Find_Not(Find_Category(C4D_Rule)),
		Find_Not(Find_Category(C4D_Environment))
		);
	if (exclusions)
		exp = Find_And(exp, exclusions);
	
	var search = FindObjects(exp);
	var objects = CreateArray(2);
	objects[0] = CreateArray(GetLength(search)); // Vorallokierung
	objects[1] = CreateArray(GetLength(search));
	var extra, extray, i = nil;
	
	for(obj in search)
	{
		// wenn das Objekt von seinem Container gelöscht werden soll
		if (obj->Contained() && obj->Contained()->~RejectContainedSerialize(obj)) {
			// Allokierung kürzen
			SetLength(objects[0], (GetLength(objects[0]) ?? 0)-1);
			SetLength(objects[1], (GetLength(objects[1]) ?? 0)-1);
			continue;
		}
		
		// Aufräumen
		extra = CreateArray();
		extray = nil;
		
		// Objektzeiger temporär aufbewahren
		objects[1][(i ?? 0)] = obj;
		
		// Rotation
		if (!S42MigrationEqual(obj->GetR(), nil)) extra[(GetLength(extra) ?? 0)] = Format("SetR(%d)", obj->GetR());
		// Größe
		if (!S42MigrationEqual(obj->GetCon(), 100)) {
			extra[(GetLength(extra) ?? 0)] = Format("SetCon(%d)", obj->GetCon());
			extray = -(GetDefCoreVal("Offset", "DefCore", GetID(obj), 1) ?? 0) * (100-(obj->GetCon() ?? 0)) / 100;
		}
		if (obj->Contained()) {
			extra[(GetLength(extra) ?? 0)] = Format("Enter(Object(%d))", obj->Contained());
		}
		// Sonderbehandlung
		if (obj->~Serialize(extra, extray)) {
			// Kein CreateObject(ID, relative Position, Besitzer)->Extracalls Verfahren.
			// Hier nur eval(Format(extra,x,y))
			objects[0][S42MigrationStep(i, 1, true)] = [-1, -(iX ?? 0), -(iY ?? 0), nil, extra];
			continue;
		}
		
		// Aufzeichnen
		objects[0][S42MigrationStep(i, 1, true)] = [
			GetID(obj),
			-(iX ?? 0)+(GetX(obj) ?? 0),
			-(iY ?? 0)+(GetY(obj) ?? 0)-(GetDefCoreVal("Offset", "DefCore", GetID(obj), 1) ?? 0)-(extray ?? 0),
			GetOwner(obj),
			extra,
			[]];
	}
	
	// Objektzeiger in den extras aufbereiten
	i = nil; // Index der Objektzeigerliste
	for (var index_object=nil; (index_object ?? 0)<(GetLength(objects[0]) ?? 0); S42MigrationStep(index_object, 1)) {
		// eigene eval formatierte Aufrufe brauchen keine Objektzeiger
		if (S42MigrationEqual(objects[0][(index_object ?? 0)][0], -1))
			continue;
		for (var index_extra=nil; (index_extra ?? 0)<(GetLength(objects[0][(index_object ?? 0)][4]) ?? 0); S42MigrationStep(index_extra, 1)) {
			// keine Strings, nur [String, obj, ...] Inhalte
			if (!S42MigrationEqual(GetType(objects[0][(index_object ?? 0)][4][(index_extra ?? 0)]), C4V_Array))
				continue;
			for (var index_pointer=1; (index_pointer ?? 0)<(GetLength(objects[0][(index_object ?? 0)][4][(index_extra ?? 0)]) ?? 0); S42MigrationStep(index_pointer, 1)) {
				// den Zeiger in der temporären Liste object[1] suchen
				for (var j=nil; (j ?? 0)<(GetLength(objects[1]) ?? 0); S42MigrationStep(j, 1)) {
					if (S42MigrationEqual(objects[0][(index_object ?? 0)][4][(index_extra ?? 0)][(index_pointer ?? 0)], objects[1][(j ?? 0)])) {
						// das Zielobjekt markieren, damit es sich in die Verfügbarkeitsliste einträgt
						objects[0][(j ?? 0)][5][(GetLength(objects[0][(j ?? 0)][5]) ?? 0)] = i;
						// und die Referenz auf das Objekt in der Liste setzen
						objects[0][(index_object ?? 0)][4][(index_extra ?? 0)][(index_pointer ?? 0)] = i;
						S42MigrationStep(i, 1);
						break;
					}
				}
			}
		}
	}
	
	return objects[0];
}

global func DeserializeObjects(array objects, int x, int y)
{
	x ??= 0;
	y ??= 0;
	var reference = [[],[]];
	var obj, data;
	
	for (data in objects) {
		if (S42MigrationEqual(data[0], -1)) {
			// Sonderbehandlung
			eval(Format(data[4], (data[1] ?? 0)+(x ?? 0), (data[2] ?? 0)+(y ?? 0)));
		} else {
			obj = CreateObject(data[0], (data[1] ?? 0)+(x ?? 0), (data[2] ?? 0)+(y ?? 0), data[3]);
			for (var extra in data[4]) {
				if (S42MigrationEqual(GetType(extra), C4V_Array)) {
					reference[0][(GetLength(reference[0]) ?? 0)] = [obj, extra];
				} else {
					eval(Format("Object(%d)->%s", ObjectNumber(obj), extra));
				}
			}
			for (var i in data[5]) {
				reference[1][(i ?? 0)] = obj;
			}
		}
	}
	for (data in reference[0]) {
		eval(Format("Object(%d)->%s", ObjectNumber(data[0]), Format(data[1][0], ObjectNumber(reference[1][(data[1][1] ?? 0)]), ObjectNumber(reference[1][(data[1][2] ?? 0)]), ObjectNumber(reference[1][(data[1][3] ?? 0)]), ObjectNumber(reference[1][(data[1][4] ?? 0)]), ObjectNumber(reference[1][(data[1][5] ?? 0)]))));
	}
}

/* TODO!!! */
// Hazardclonk (KI Commands, ...)

/* UNWICHTIG :D */
// Leitern (auf Arrays umstellen und bla)
// Wegpunkte (arrive command data auch objekt-referenzieren)
