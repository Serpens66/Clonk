/*--- Bausatz ---*/

#strict


local helper,clonksize;

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  // Clonk anhalten
  SetComDir(COMD_Stop(), pClonk);
  // Kann der Clonk überhaupt bauen?
  if (pClonk && !GetPhysical("CanConstruct", PHYS_Current, pClonk))
    { PlayerMessage(GetController(pClonk), "$TxtCantConstruct$", this, GetName(pClonk)); return false; }
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoconstructionplansa$");
  var idType; var i = 0;

  helper=CreateObject(A_CN,0,0,GetOwner(pClonk));
  SetVisibility(VIS_Owner(),helper);
  clonksize=GetDefCoreVal("Offset",0,GetID(pClonk),1);
  var smec=CreateObject(SM3C);
  smec->Init(pClonk,this(), "ShowBuidling", 1);

  while (idType = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_Structure()))
    // Keine Trapper/Indianer Sachen mit Bausatz bauen. (Außer für Wachtürme/Palisaden, die es ausdrücklich zulassen)
    if(idType->~IsConkitBuilding() || (!idType->~IsIndianHandcraft() && !idType->~IsTrapperHandcraft()))
    if (GetDefConstruction(idType))    // nur Gebäude mit Construction=1
      //AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);

   {
       
      if(GetLength(hashSpielRegeln))  // baustellen kosten clunker?
        {
            var ProdCostAnzeige = 0;
              ProdCostAnzeige = (GetMass(0,idType)/10) * (HashGet(hashSpielRegeln, "Baustellenkosten")/100);  // standardkosten sind also die Masse des Gebäudes/10
            if (HashGet(hashSpielRegeln, "Baustellenkosten"))
                AddMenuItem(Format("%d C. %s",ProdCostAnzeige,GetName(0,idType)), "CreateConstructionSite2", idType, pClonk,0,pClonk);
            else
                AddMenuItem("$TxtConstructions$", "CreateConstructionSite2", idType, pClonk,0,pClonk);
        }
      else
        AddMenuItem("$TxtConstructions$", "CreateConstructionSite2", idType, pClonk,0,pClonk);
              
    smec->AddInfo(idType);
    }
  return(1);
}


// gebäude vorher anzeigen:
protected func CreateConstructionSite2(idType,caller)
{
  ShowBuidling_Closed();
  ProtectedCall(this(),"CreateConstructionSite",idType,caller);
}

func ShowBuidling_Changed(selection,idinfo){
SetGraphics(0,helper,idinfo,1,GFXOV_MODE_Base);
SetPosition(GetX(),GetY()+GetDefCoreVal("Offset",0,idinfo,1)-clonksize,helper);
Var(0)=GetX(); Var(1)=GetY();
FindConstructionSite(idinfo,0,1); 
if(GetX()==Var(0) || GetY()==Var(1))SetClrModulation(RGBa(100,250,100,70),helper,1);
 else SetClrModulation(RGBa(250,100,100,70),helper,1);
}

func ShowBuidling_Closed(){
if(helper)RemoveObject(helper);
}





protected func CreateConstructionSite(idType,caller) // wird durch appendto im EPIC teil überschrieben !!!
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(Contained()) ne "Walk") return(0);
  if (Contained(Contained())) return(0);
  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(0, 10, Contained()) ) return(0);
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  // Baustelle erschaffen
  var pSite;
  if (!(pSite = CreateConstruction(idType, 0, 10, GetOwner(Contained()), 1, 1,1))) return(0);
  // Meldung
  Message("$TxtConstructions$", Contained(), GetName(pSite));
  // Bausatz verbrauchen
  RemoveObject();
  return(1);
}

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
