/*-- Leitungsbausatz --*/

#strict 3

local pBuilding,Local2,Local3;

func Initialize() 
{

  Local(2)=1;
  Local(3)=1;
  Local2=1;
  Local3=1;

  pBuilding = FindObject2(Find_AtPoint(),Find_Category(C4D_Structure));
  
  if(FindObject(PWRL,nil,nil,nil,nil,nil,"Connect",pBuilding)) Schedule("CheckLine()",1);

}

func CheckLine() // um zu bemerken, wann eine Leitung von etwas entfernt wurde.
{
    pBuilding = FindObject2(Find_AtPoint(),Find_Category(C4D_Structure));
    //Message("%s",0,GetName(pBuilding));
    // Leitungsbausatz hat beim erzeugen eine Energieleitung? --> Muss gerade abgenommen worden sein! (Danke Clonkonaut)
    var pLine = FindObject(PWRL,nil,nil,nil,nil,nil,"Connect",this());
    if(pLine) 
    {
        pLine->LostConnection(pBuilding); // fuer umspannwerk
        // Log(Format("Checkline Lost building %v",GetName(pBuilding)));
        if(FindObject(NLEI)) // wenn Regel, dass Feinde keine Leitungen abnehmen duerfen
            if(Contained())  
                if(Hostile(GetOwner(Contained()),GetOwner(pBuilding))) // Wenn Clonk und gebädue verfeindet
                {
                    Activate(Contained(),99); // dann Leitung sofort wieder verbinden, mit 99 um ausnahmsweise durch einen feind verbinden
                    Sound("Error");
                    return(Message("Keine Leitungsinteraktionen bei Feinden möglich", pBuilding) );
                }
    }
}

protected func Hit()
{
  Sound("RockHit*");
}

protected func Activate(clonk,iErlaubt)
{
  [$TxtConnectline$]
  // Objekt zum Line-Verlegen finden.
  var obj = FindObject(nil, 1,nil,nil,nil, OCF_LineConstruct, nil,nil,nil, obj);
  // Keins da? Auch gut.
  if(!obj) return(Message("$TxtNoNewLine$", this()) );
  
  if(And(FindObject(NLEI), !Equal(iErlaubt, 99))) // wenn Regel, dass Feinde keine Leitungen anbringen duerfen, und es nicht gerade durch das sofortige wieder dranpacken ausgelöst wurde
      if(Hostile(GetOwner(obj),GetOwner(clonk))) 
      {
          Sound("Error");
          return(Message("Keine Leitungsinteraktionen bei Feinden möglich", obj) );
      }
    
  
  // Leitung
  var pLine = FindObject(nil, nil,nil,nil,nil, nil, "Connect", this() );
  SetComDir(COMD_Stop, clonk);

  // Aktuelle Leitung anschließen
  if(pLine)
  {
    // Zielobjekt == Quellobject?
    if(Or(Equal(obj, GetActionTarget(nil, pLine)), Equal(obj, GetActionTarget(1, pLine))) )
    {
      // Leitung wieder entfernen
      Message("$TxtLineRemoval$", this(), GetName(pLine) );
      RemoveObject(pLine);
      Sound("Connect");
      return(1);
    }
    else
    {
      // Line an obj anschließen
      if(!!SEqual(Format("%i",GetID(obj)), "_USW")) obj->ConnectionTarget(GetActionTarget(1, pLine),1);
      if(!!SEqual(Format("%i",GetID(obj)), "_USW")) obj->ConnectionTarget(GetActionTarget(nil, pLine),nil);
      if(!!SEqual(Format("%i",GetID(GetActionTarget(nil, pLine))), "_USW")) GetActionTarget(nil, pLine)->ConnectionTarget(obj,1);
      if(!!SEqual(Format("%i",GetID(GetActionTarget(1, pLine))), "_USW")) GetActionTarget(1, pLine)->ConnectionTarget(obj,nil);
      if(!ConnectLine(pLine, obj) ) return(1);
    }
  }
  // Neue Leitung
  else
  {
    // LineType abfragen
    var linetype = obj->~LineConnectType();
    //linetype = PWRL;
    // Nicht vorhanden -> Standard-LineType verwenden
    if(!linetype) linetype = DefaultLineType(obj);
    // Kein möglicher Leitungstyp? :(
    if(!linetype)
    {
      Sound("Error");
      return(Message("$TxtNoNewLine$", this()) );
    }

    // Line erzeugen
    pLine = CreateLine(linetype, GetOwner(), obj, this() );
  }

  // Engineinternes Verhalten blockieren
  return(1);
}

private func DefaultLineType(obj)
{
  // LineConnect-Wert abfragen
  var line_accept = GetDefCoreVal("LineConnect", "DefCore", GetID(obj) );

  // Pumpe
  if (And((line_accept ?? 0) & (C4D_LiquidPump ?? 0), (line_accept ?? 0) & (C4D_LiquidInput ?? 0)))
  {
    var nothing = true;
    for (var pipe in FindObjects(Find_ID(SPIP), Find_ActionTarget(obj), Find_Action("Connect")))
    {
      // Nur das erste Actiontarget zaehlt.
      if (Equal(pipe->GetActionTarget(nil), obj))
        nothing = nil;
    }
    if (nothing) return(SPIP);
  }
  if (And((line_accept ?? 0) & (C4D_LiquidPump ?? 0), (line_accept ?? 0) & (C4D_LiquidOutput ?? 0)))
  {
    nothing = true;
    for (pipe in FindObjects(Find_ID(DPIP), Find_ActionTarget(obj), Find_Action("Connect")))
    {
      if (Equal(pipe->GetActionTarget(nil), obj))
        nothing = nil;
    }
    if (nothing) return(DPIP);
  }

  // Strom
  if((line_accept ?? 0) & (C4D_PowerOutput ?? 0))
    return(PWRL);

  // Gar nix, dann werden manche Leitungen eventuell doppelt.
  if((line_accept ?? 0) & (C4D_LiquidPump ?? 0))
    return(SPIP);
  return(nil);
}

private func CreateLine(linetype, owner, from, to)
{
  var line = CreateObject(linetype, nil, nil, owner);
  if(!!SEqual(Format("%i",GetID(from)), "_USW"))
    SetLocal(2,1,line);
  ObjectSetAction(line, "Connect", from, to);
  Message("$TxtNewLine$", this(), GetName(line) );
  Sound("Connect");
  return(line);
}

/* Verbinden zweier Gebäude mit einer Leitung */
private func ConnectLine(line, to)
{
  /* Leitungstyp der Leitung und Anschlussmöglichkeit am Ziel finden */
  var line_type = GetDefCoreVal("Line", "DefCore", GetID(line) );
  var line_accept = GetDefCoreVal("LineConnect", "DefCore", GetID(to) );
  
  // Targets ausmachen. Es ist nicht definiert, welches Target das aktuell
  // verbundene Objekt und welches das Linekit ist.
  var t1 = GetActionTarget(nil, line);
  var t2 = to;
  if(Equal(t1, this()) ) { t2 = GetActionTarget(1, line); t1 = to; }
  
  var ok = nil;
  // Energieleitung?
  if (Or(Equal(line_type, C4D_LinePower), Equal(line_type, C4D_LineVertex)))
  {
    // Geht ok, wenn das Ziel Strom benötigt oder welchen hergibt
    if (And(Equal(t1, to), (line_accept ?? 0) & (C4D_PowerOutput ?? 0)))
      ok = true;
    if (And(Equal(t2, to), (line_accept ?? 0) & (C4D_PowerInput ?? 0)))
      ok = true;
  }
  // Zufluss?
  else if(Equal(line_type, C4D_LineSource))
  {
    // Geht ok, wenn das Ziel Flüssigkeiten benötigt oder hergibt
    if (And(And(Equal(t1, to), (line_accept ?? 0) & (C4D_LiquidPump ?? 0)), (line_accept ?? 0) & (C4D_LiquidInput ?? 0)))
    {
      ok = true;
      // Jede Pumpe kann nur eine Zuflussleitung auf einmal haben.
      for (var pipe in FindObjects(Find_ID(SPIP), Find_ActionTarget(to), Find_Action("Connect")))
      {
        if (Equal(pipe->GetActionTarget(nil), to))
          ok = nil;
      }
    }
    if (And(Equal(t2, to), (line_accept ?? 0) & (C4D_LiquidOutput ?? 0)))
      ok = true;
  }
  // Abfluss?
  else if(Equal(line_type, C4D_LineDrain))
  {
    // Geht ok, wenn das Ziel Flüssigkeit hergibt oder benötigt
    if (And(And(Equal(t1, to), (line_accept ?? 0) & (C4D_LiquidPump ?? 0)), (line_accept ?? 0) & (C4D_LiquidOutput ?? 0)))
      ok = true;
    if (And(Equal(t2, to), (line_accept ?? 0) & (C4D_LiquidInput ?? 0)))
      ok = true;
  }

  // Kann die Leitung hier angeschlossen werden?
  if(!ok)
  {
    // Nö, also tschüss
    Sound("Error");
    Message("$TxtNoConnectType$", this(), GetName(line), GetName(to) );
    return(nil);
  }
  
  // Verbinden
  ObjectSetAction(line, "Connect", t1, t2);
  if(!!SEqual(Format("%i",GetID(t2)), "_USW"))
    SetLocal(3,1,line);

  Message("$TxtConnect$", Contained(), GetName(line), GetName(to) );
  Sound("Connect");
  RemoveObject();
  return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
