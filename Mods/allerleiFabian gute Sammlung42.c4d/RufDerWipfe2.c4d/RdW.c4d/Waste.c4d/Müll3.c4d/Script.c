#strict

local fightbonus, engineerbonus, researchbonus;// Zur Wiederbelebung 


func SetLeiche(pClonk)
{
// Items von clonk aufnehmen
GrabContents(pClonk);

var stuuufe= (pClonk->~AktuelleStufe());
  if(GetID(pClonk)==CLNK)
  {
  SetClrModulation(GetClrModulation(pClonk));
  SetColorDw(GetColorDw(pClonk));
  SetObjectBlitMode(GetObjectBlitMode(pClonk));
  SetName(GetName(pClonk));
  //Log("clonkleichengrafik");
  }
if(GetID(pClonk)==HZCK)
 {
   // wenn Hazardclonkgrafik, also kein Bauarbeiter und stufe zwischen 15 und 19. Oder beim Start erstellter Hazard.
   //if((!(pClonk->~IstBauarbeiter()) && stuuufe>=15 && stuuufe<20) || stuuufe==0)
   //{
  SetClrModulation(GetClrModulation(pClonk));
  SetColorDw(GetColorDw(pClonk));
  SetObjectBlitMode(GetObjectBlitMode(pClonk));
  var grafik = GetGraphics(pClonk,1);
  if(grafik=="") SetGraphics("LieHazard");
  if(grafik=="Future") SetGraphics("FutureLie");
  if(grafik=="Bauer") SetGraphics("BauerLie");
  if(grafik=="SG1") SetGraphics("SG1Lie");
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
  if(!Collect(this(),pClonk))Sound("CommandFailure1*",0,pClonk);
  SetAlive(0);
  return(1);
}


public func Hit()
{
  Sound("FlourHit");
}


public func IsWaste(){if(FindObject(NOWS)) return(0);return(40);} // wenn kein müll regel, dann kein umweltschaden. Leiche verwest mit der zeit eh
public func PaketID(){ return(0);  }

func Verwesung()
{
if(FindObject(NOWS)) DoCon(-10); // doppelt so schnelle verwesung, wenn kein müll regel aktiv
DoCon(-10);
return();
}
