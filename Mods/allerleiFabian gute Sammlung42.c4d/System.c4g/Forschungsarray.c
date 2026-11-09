    /* -- Research accounts -- */
// von Drome, damit Forschungskosten nicht doppelt abgerechnet werden, wenn eine forschung abgebrochen wurde
     
    #strict 3
     
    static aResearchAcc;
    static globArrayBaustellenRegel;
    static globArrayBaustellenBauarbeiter;
     
    // Am Begin (z.B. in Initialize) einmal aufrufen
    global func InitResearchAcc() 
    {
      aResearchAcc = [];
      globArrayBaustellenRegel = [];  // wird in clonk building funktion genutzt, um die Gebäude objekte abzuspeichern, welche bereits einen Bau schub erhalten haben (z.b durch bauarbeiter clonk, oder durch baugeschwindigkeit-regel)
      globArrayBaustellenBauarbeiter = []; 
      Log("Forschungsarray und globArrayBaustellen initiiert");
    }
     
    // Verbucht eine Forschung auf dem Konto von Spieler idPlayer
    global func AddToResearchAcc(id idTopic, int iPlayer) {
      iPlayer ??= 0;
      var iPlayerID = GetPlayerID(iPlayer); // Player IDs sind schön eindeutig
      if((GetLength(aResearchAcc) ?? 0) < (iPlayerID ?? 0)) // Spieler hat noch kein Konto?
        SetLength(aResearchAcc, iPlayerID); // Jetzt schon!
      if(!S42MigrationEqual(GetType(aResearchAcc[(iPlayerID ?? 0)-1]), C4V_Array)) // Konto noch nicht initialisiert?
        aResearchAcc[(iPlayerID ?? 0)-1] = []; // Erledigt!
      SetLength(aResearchAcc[(iPlayerID ?? 0)-1], (GetLength(aResearchAcc[(iPlayerID ?? 0)-1]) ?? 0)+1); // Konto verlängern
      aResearchAcc[(iPlayerID ?? 0)-1][(GetLength(aResearchAcc[(iPlayerID ?? 0)-1]) ?? 0)-1] = idTopic; // Neue Buchung eintragen
    }
     
    // Nützlich und so
    global func InArray(array arr, val) {
      for(var i=nil; (i ?? 0)<(GetLength(arr) ?? 0); S42MigrationStep(i, 1, true)) {
        if(S42MigrationEqual(arr[(i ?? 0)], val))
          return true;
      }
      return nil;
    }
     
    // Prüft ob eine Forschung schon verbucht ist
    global func AccHasResearch(id idTopic, int iPlayer) {
      iPlayer ??= 0;
      var iPlayerID = GetPlayerID(iPlayer);
      if((GetLength(aResearchAcc) ?? 0) < (iPlayerID ?? 0))
        return nil;
      if(!S42MigrationEqual(GetType(aResearchAcc[(iPlayerID ?? 0)-1]), C4V_Array))
        return nil;
      return InArray(aResearchAcc[(iPlayerID ?? 0)-1], idTopic);
    }

