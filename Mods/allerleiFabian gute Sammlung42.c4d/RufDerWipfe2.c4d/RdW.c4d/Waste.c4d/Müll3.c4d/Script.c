#strict 3

local fightbonus, engineerbonus, researchbonus;// Zur Wiederbelebung 


func SetLeiche(pClonk)
{
// Items von clonk aufnehmen
GrabContents(pClonk);

var stuuufe= (pClonk->~AktuelleStufe());
  if(Equal(GetID(pClonk), CLNK))
  {
  SetClrModulation(GetClrModulation(pClonk));
  SetColorDw(GetColorDw(pClonk));
  SetObjectBlitMode(GetObjectBlitMode(pClonk));
  SetName(GetName(pClonk));
  //Log("clonkleichengrafik");
  }
if(Equal(GetID(pClonk), HZCK))
 {
   // wenn Hazardclonkgrafik, also kein Bauarbeiter und stufe zwischen 15 und 19. Oder beim Start erstellter Hazard.
   //if((!(pClonk->~IstBauarbeiter()) && stuuufe>=15 && stuuufe<20) || stuuufe==0)
   //{
  SetClrModulation(GetClrModulation(pClonk));
  SetColorDw(GetColorDw(pClonk));
  SetObjectBlitMode(GetObjectBlitMode(pClonk));
  var grafik = GetGraphics(pClonk,1);
  if(Equal(grafik, "")) SetGraphics("LieHazard");
  if(Equal(grafik, "Future")) SetGraphics("FutureLie");
  if(Equal(grafik, "Bauer")) SetGraphics("BauerLie");
  if(Equal(grafik, "SG1")) SetGraphics("SG1Lie");
  // usw.
  SetName(GetName(pClonk));
  //Log("hazardleichengrafik");
   //}
 }
if(!pClonk)
Log("kein Ziel für Leichenart");

}

func ControlDigDouble(pClonk)
{
  [Einsammeln]
  if(!Collect(this(),pClonk))Sound("CommandFailure1*",nil,pClonk);
  SetAlive(nil);
  return(1);
}


public func Hit()
{
  Sound("FlourHit");
}


public func IsWaste(){if(FindObject(NOWS)) return(nil);return(40);} // wenn kein müll regel, dann kein umweltschaden. Leiche verwest mit der zeit eh
public func PaketID(){ return(nil);  }

func Verwesung()
{
if(FindObject(NOWS)) DoCon(-10); // doppelt so schnelle verwesung, wenn kein müll regel aktiv
DoCon(-10);
return;
}
