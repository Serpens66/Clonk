    /* -- Research accounts -- */
// von Drome, damit Forschungskosten nicht doppelt abgerechnet werden, wenn eine forschung abgebrochen wurde
     
    #strict 2
     
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
      var iPlayerID = GetPlayerID(iPlayer); // Player IDs sind schön eindeutig
      if(GetLength(aResearchAcc) < iPlayerID) // Spieler hat noch kein Konto?
        SetLength(aResearchAcc, iPlayerID); // Jetzt schon!
      if(GetType(aResearchAcc[iPlayerID-1]) != C4V_Array) // Konto noch nicht initialisiert?
        aResearchAcc[iPlayerID-1] = []; // Erledigt!
      SetLength(aResearchAcc[iPlayerID-1], GetLength(aResearchAcc[iPlayerID-1])+1); // Konto verlängern
      aResearchAcc[iPlayerID-1][GetLength(aResearchAcc[iPlayerID-1])-1] = idTopic; // Neue Buchung eintragen
    }
     
    // Nützlich und so
    global func InArray(array arr, val) {
      for(var i=0; i<GetLength(arr); i++) {
        if(arr[i] == val)
          return true;
      }
      return false;
    }
     
    // Prüft ob eine Forschung schon verbucht ist
    global func AccHasResearch(id idTopic, int iPlayer) {
      var iPlayerID = GetPlayerID(iPlayer);
      if(GetLength(aResearchAcc) < iPlayerID)
        return false;
      if(GetType(aResearchAcc[iPlayerID-1]) != C4V_Array)
        return false;
      return InArray(aResearchAcc[iPlayerID-1], idTopic);
    }

