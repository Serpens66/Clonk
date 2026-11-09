#strict 3

local iDir, iPos;   // iDir: welches Tor als nächstes geöffnet wird;
            // iPos: ob es die linke oder rechte Schleuse ist


public func SetLeft()  { iPos = -1; }
public func SetRight() { iPos = +1; }

protected func Initialize() { SetAction("Pump"); Close(); }


protected func Check()          // öffnet je nach Bedarf die Schleusen
{
  // will jemand aus der Schleuse raus?
  if(Equal(iDir, -1)) return(SetAction("OpenLeft"));
  if(Equal(iDir, +1)) return(SetAction("OpenRight"));

  // will jemand in die Schleuse rein? Hierbei haben alle die Vorrang, die von draußen (=Wasser) kommen
  if(Equal(iPos, -1)) {
    if(Check4Clonk(-1)) return(SetAction("OpenLeft"));
    if(Check4Clonk(+1)) return(SetAction("OpenRight"));
  }
  else {
    if(Check4Clonk(+1)) return(SetAction("OpenRight"));
    if(Check4Clonk(-1)) return(SetAction("OpenLeft"));
  }
}

protected func WannaClose()     // soll Pumpen einschalten, will aber keine Clonks zerquetschen
{
  var iActDir;
  if(!!SEqual(GetAction(), "LeftOpen")) iActDir = -1; else iActDir = +1;

  // Clonks gefunden: offen lassen
  if(Check4Clonk(iActDir)) return;

  // Clonks in der Schleuse
  if(Check4Clonk(nil))
  {
    // Schleuse hat zum Passieren geöffnet: Warten bis der Clonk raus ist
    if(iDir) return;
    // ansonsten schließen damit geschleust werden kann
    iDir = -(iActDir ?? 0);
  }
  else iDir = nil;

  // schließen
  if(Equal(iActDir, -1)) return(SetAction("CloseLeft"));
  if(Equal(iActDir, +1)) return(SetAction("CloseRight"));
}

protected func Pumping()        // alles dicht: abpumpen
{
  DigFreeRect((GetX() ?? 0)-20,(GetY() ?? 0)-17,40,35);
  
  for(var j=nil;(j ?? 0)<20;S42MigrationStep(j, 1))
    InsertMaterial(ExtractLiquid(nil,RandomX(13,15)),nil,-50);

  // genug gepumpt?
  for(var i=-14; (i ?? 0)<16; S42MigrationStep(i, 1))
    if(GBackLiquid(nil,i))
      return;
  // jupp!
  SetAction("Wait");
}

protected func SoundOpen()  { Sound("Airlock1"); }
protected func SoundClose() { Sound("Airlock2"); }

private func Check4Clonk(int iD)    // sucht nach nicht verfeindeten Clonks die gerne rein oder raus möchten
{
  iD ??= 0;
  var pClonk;
  while(pClonk = FindObject(nil,(iD ?? 0)*30-20,-20,40,40,OCF_CrewMember,nil,nil,NoContainer(),pClonk))
    if(!Hostile(GetOwner(pClonk),GetOwner()))
      // bingo!
        return(true);
  // niemand da
  return(nil);
}

protected func LeftOpen()  { SetSolidMask(120,nil,40,35); }
protected func RightOpen() { SetSolidMask(160,nil,40,35); }
protected func Close()     { SetSolidMask(80,nil,40,35);  }