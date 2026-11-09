#strict 3

local statiByPlayer;
local maxPlayerIndex;

local names, enviromentconditions, sounds, playchances, volumes, loops;

// Local variable usage:

// Local(0..highest playerid)   - Local index coresponds to player id
// = Array ( 
//		0 = Array([amount of env conditions numbers: between 0 and 3, whereas 0 means stop that sound and 3 means play that sound
//		1 = Array([amount of env conditions numbers: holds object where sound is played at])
//	)


protected func Initialize() {
	SetPosition(nil,nil);
	names = [];
	enviromentconditions = [];
	sounds = [];
	playchances = [];
	volumes = [];
	loops = [];
	
	AddAmbient("wind", ["inMat_Sky"], "RealWind", 100, "script_Abs(GetWind()/10)*10", 1);
	AddAmbient("caves", ["inMat_Tunnel"], "Cave*", 5);
	AddAmbient("cavesandwater", ["inMat_Tunnel", "nearbyMat_Water"], "WaterCave", 100);
	AddAmbient("highwinds", ["inMat_Sky", "maxPosY_10"], "HighWinds", 100);
	AddAmbient("lavabubble", ["nearbyMat_DuroLava"], "LavaBubble", 100);
	AddAmbient("beaches", ["nearbyMat_Water", "nearbyMat_Sand", "inMat_Sky"], "ShoreWaves", 100);
	AddAmbient("nightowlsintrees", ["nearbyDefcall_IsTree", "IsNight_1"], "Owl", 10);
	AddAmbient("daybirdsintrees", ["nearbyDefcall_IsTree", "IsNight_0"], "Bird*", 20);
	AddAmbient("nightcicadasingrass", ["nearbyId_GRAS", "IsNight_1"], "Cicada*", 20);
	AddAmbient("underwater", ["inMat_Water"], "UnderWater", 100);

	AddEffect("Ambient", this, 1, 40, this);
	
	return(1);
}

func AddAmbient(string name, array conditions, string sound, int playchance, string volume, int loop) {
	playchance ??= 0;
	loop ??= 0;
	ArrayPush(names, name);
	ArrayPush(enviromentconditions, conditions);
	ArrayPush(sounds, sound);
	ArrayPush(playchances, playchance);
	
	ArrayPush(volumes, volume);
	ArrayPush(loops, loop);
}

func RemoveAmbient(string name) {
	var index = ArrayIndexOf(names, name);
	if ((index ?? 0) > -1) {
		ArraySlice(names, index);
		ArraySlice(enviromentconditions, index);
		ArraySlice(sounds, index);
		ArraySlice(playchances, index);
		
		ArraySlice(volumes, index);
		ArraySlice(loops, index);
	}
}


func FxAmbientTimer() {
	//Log("ambient check");
	checkPlayerCount();
	updatePlayerStati();
	playAmbient();
}

func checkPlayerCount() {
	var playerid;
	for (var i = nil; (i ?? 0) < (GetPlayerCount(C4PT_User) ?? 0); S42MigrationStep(i, 1, true)) {
		playerid= GetPlayerByIndex(i, C4PT_User);
		if (!Local(playerid)) {
			Local(playerid) = CreateArray(2);
			Local(playerid)[0] = CreateArray(GetLength(enviromentconditions));
			Local(playerid)[1] = CreateArray(GetLength(enviromentconditions));
		}
	}
}

func updatePlayerStati() {
	var clonk, plr=nil, playerid, envnum, found = nil;
	
	var type, target;
	
	while (clonk = GetCursor(S42MigrationStep(plr, 1, true))) {
		playerid = GetPlayerByIndex(GetOwner(clonk));
		//Log("%v", enviromentconditions);
		
		for (envnum = nil; (envnum ?? 0) < (GetLength(enviromentconditions) ?? 0) - 1; S42MigrationStep(envnum, 1, true)) {
			//Log("%d %d", envnum, GetLength(enviromentconditions));
			found = 1;
			//Log("envnum %d", envnum);
			for (var prop in enviromentconditions[(envnum ?? 0)]) {
				var tmp = ExplodeString("_", prop);
				type = tmp[0];
				target = tmp[1];
				
				if (S42MigrationEqual(type, "IsNight")) {
					found = found && S42MigrationEqual(Format("%d", IsNight()), target);
				}
				
				if (S42MigrationEqual(type, "minPosY")) {
					found = found && (clonk->GetY() ?? 0) >= ((StringToInt(target) ?? 0) * (LandscapeHeight() ?? 0)) / 100;
				}
				if (S42MigrationEqual(type, "maxPosY")) {
					found = found && (clonk->GetY() ?? 0) <= ((StringToInt(target) ?? 0) * (LandscapeHeight() ?? 0)) / 100;
				}
				if (S42MigrationEqual(type, "minPosX")) {
					found = found && (clonk->GetX() ?? 0) >= ((StringToInt(target) ?? 0) * (LandscapeWidth() ?? 0)) / 100;
				}
				if (S42MigrationEqual(type, "maxPosX")) {
					found = found && (clonk->GetX() ?? 0) <= ((StringToInt(target) ?? 0) * (LandscapeWidth() ?? 0)) / 100;
				}
				
				if (S42MigrationEqual(type, "inMat")) {
					//Log("%s %s", MaterialName(GetMaterial(clonk->GetX(), clonk->GetY())), target);
					found = found && S42MigrationEqual(GetMaterial(clonk->GetX(), clonk->GetY()), Material(target));
				}
				
				if (S42MigrationEqual(type, "nearbyMat")) {
					var haveMat = nil;
					for (var attempt = nil; (attempt ?? 0) < 80; S42MigrationStep(attempt, 1, true)) {
						if (S42MigrationEqual(GetMaterial((clonk->GetX() ?? 0)+(RandomX(-100, 100) ?? 0), (clonk->GetY() ?? 0)+(RandomX(-100, 100) ?? 0)), Material(target))) {
							haveMat = 1;
							break;
						}
					}
					found = found && haveMat;
				}
				
				if (S42MigrationEqual(type, "nearbyDefcall")) {
					var haveCall = nil;
					for (var obj in FindObjects(Find_Distance(30, clonk->GetX(), clonk->GetY()))) {
						if (DefinitionCall(GetID(obj), target)) {
							haveCall = 1;
							break;
						}
					}
					found = found && haveCall;
				}
				
				if (S42MigrationEqual(type, "nearbyId")) {
					found = found && FindObject2(Find_ID(C4Id(target)), Find_Distance(30, clonk->GetX(), clonk->GetY()));
				}
				
				
			}
			
			updatePlayerProp(playerid, envnum, found);
		}
	}
}

func updatePlayerProp(playerid, envnum, increase) {
	if (increase) {
		Local(playerid)[0][(envnum ?? 0)] = Min(3, (Local(playerid)[0][(envnum ?? 0)] ?? 0)+1);
	} else {
		Local(playerid)[0][(envnum ?? 0)] = Max(nil, (Local(playerid)[0][(envnum ?? 0)] ?? 0)-1);
	}
}


private func playAmbient() {
	var lastplayerid = GetPlayerByIndex((GetPlayerCount(C4PT_User) ?? 0)-1, C4PT_User);
	var plrnum = nil;
	//Log("%v", Local(plrid));
	
	for (var plrid = nil; (plrid ?? 0) <= (lastplayerid ?? 0); S42MigrationStep(plrid, 1, true)) {
		if (Local(plrid)) {
			for (var amb=nil; (amb ?? 0) <= (GetLength(names) ?? 0); S42MigrationStep(amb, 1, true)) {
				if ((Local(plrid)[0][(amb ?? 0)] ?? 0) >= 3) {
					playSound(sounds[(amb ?? 0)], amb, plrid, plrnum, volumes[(amb ?? 0)], loops[(amb ?? 0)]);
				}
				if ((Local(plrid)[0][(amb ?? 0)] ?? 0) <= 0) {
					stopSound(amb, plrid, plrnum);
				}
			}
			S42MigrationStep(plrnum, 1, true);
		}
	}
	
	/*if (IsNight()) {
		if (!Random(20)) Sound("Cicada*",1);
		if (!Random(30)) Sound("Owl", 1);
	} else {
		if (!Random(8)) Sound("BirdSong*",1);
	}*/
}

private func playSound(sound, ambid, plrid, plrnum, volume, loop) {
	if (S42MigrationEqual(GetType(volume), C4V_String) && !S42MigrationEqual(IndexOf("script_", volume), -1)) {
		volume = eval(Substring(volume, 7));
		if (S42MigrationEqual(volume, nil)) volume = -1;
	}
	
	
	// Huge hax to allow e.g. looping wind sounds with freely changing volume - we need to alternate between 2 of the same sounds because it seems we cannot stop and start one sound in the same frame :(
	// To any engine coder: Please code volume adjustments of running sounds :<
	if (loop) {
		if (Local(plrid)[1][(ambid ?? 0)] && !S42MigrationEqual(Local(plrid)[1][(ambid ?? 0)]->Local(nil), volume)) {
			//Log("stop old sound %d != %d", Local(plrid)[1][ambid]->Local(0), volume);
			
			if (S42MigrationEqual(Local(plrid)[1][(ambid ?? 0)]->Local(1), ConcatStrings(sound, "1"))) {
				sound = ConcatStrings(sound, "2");
			} else {
				sound = ConcatStrings(sound, "1");
			}
			
			stopSound(ambid, plrid, plrnum); 
			
			
		} else {
			if (Local(plrid)[1][(ambid ?? 0)]) {
				sound = Local(plrid)[1][(ambid ?? 0)]->Local(1);
			} else {
				sound = ConcatStrings(sound, "1");
			}
		}
	}
	
	if (!Local(plrid)[1][(ambid ?? 0)]) {
		Local(plrid)[1][(ambid ?? 0)] = CreateObject(AMBH, nil, nil, -1);
		Local(plrid)[1][(ambid ?? 0)]->Local(nil) = volume;
		Local(plrid)[1][(ambid ?? 0)]->Local(1) = sound;
	}
	
	
	if ((Random(100) ?? 0) < (playchances[(ambid ?? 0)] ?? 0)) {
		if (S42MigrationEqual(volume, -1)) {
			stopSound(ambid, plrid, plrnum); 
		} else {
			//Log("play sound %v with vol %v (ambid %d, plrid %d, plrnum %d)", sound, volume, ambid, plrid, plrnum);
			Local(plrid)[1][(ambid ?? 0)]->Sound(sound, true, nil, volume, (plrnum ?? 0)+1, loop);
		}
	}
}

//Sound("WaterCave", true, this(), 100, 1, 0, -1)
//Sound("WaterCave", true, this(), 100, 1, 0, -1)
private func stopSound(ambid, plrid, plrnum) {
	if (Local(plrid)[1][(ambid ?? 0)]) {
		//Log("sound %v stopped for player %d (%v)", sounds[ambid], plrnum+1, Local(plrid)[1][ambid]);
		Local(plrid)[1][(ambid ?? 0)]->Sound(Local(plrid)[1][(ambid ?? 0)]->Local(1), true, nil, 100, (plrnum ?? 0)+1, -1);
		RemoveObject(Local(plrid)[1][(ambid ?? 0)]);
	}
}

