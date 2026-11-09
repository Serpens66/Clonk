/*--- Bausatz ---*/

#strict 3


local helper,clonksize;

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  // Clonk anhalten
  SetComDir(COMD_Stop, pClonk);
  // Kann der Clonk überhaupt bauen?
  if (And(pClonk, !GetPhysical("CanConstruct", PHYS_Current, pClonk)))
    { PlayerMessage(GetController(pClonk), "$TxtCantConstruct$", this, GetName(pClonk)); return nil; }
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoconstructionplansa$");
  var idType; var i = nil;

  helper=CreateObject(A_CN,nil,nil,GetOwner(pClonk));
  SetVisibility(VIS_Owner,helper);
  clonksize=GetDefCoreVal("Offset",nil,GetID(pClonk),1);
  var smec=CreateObject(SM3C);
  smec->Init(pClonk,this(), "ShowBuidling", 1);

  while (idType = GetPlrKnowledge(GetOwner(pClonk), nil, S42MigrationStep(i, 1, true), C4D_Structure))
    // Keine Trapper/Indianer Sachen mit Bausatz bauen. (Außer für Wachtürme/Palisaden, die es ausdrücklich zulassen)
    if(Or(idType->~IsConkitBuilding(), (And(!idType->~IsIndianHandcraft(), !idType->~IsTrapperHandcraft()))))
    if (GetDefConstruction(idType))    // nur Gebäude mit Construction=1
      //AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);

   {
       
      if(GetLength(hashSpielRegeln))  // baustellen kosten clunker?
        {
            var ProdCostAnzeige = nil;
              ProdCostAnzeige = ((GetMass(nil,idType) ?? 0)/10) * ((HashGet(hashSpielRegeln, "Baustellenkosten") ?? 0)/100);  // standardkosten sind also die Masse des Gebäudes/10
            if (HashGet(hashSpielRegeln, "Baustellenkosten"))
                AddMenuItem(Format("%d C. %s",ProdCostAnzeige,GetName(nil,idType)), "CreateConstructionSite2", idType, pClonk,nil,pClonk);
            else
                AddMenuItem("$TxtConstructions$", "CreateConstructionSite2", idType, pClonk,nil,pClonk);
        }
      else
        AddMenuItem("$TxtConstructions$", "CreateConstructionSite2", idType, pClonk,nil,pClonk);
              
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
SetGraphics(nil,helper,idinfo,1,GFXOV_MODE_Base);
SetPosition(GetX(),(GetY() ?? 0)+(GetDefCoreVal("Offset",nil,idinfo,1) ?? 0)-(clonksize ?? 0),helper);
Var(nil)=GetX(); Var(1)=GetY();
FindConstructionSite(idinfo,nil,1);
if(Or(Equal(GetX(), Var(nil)), Equal(GetY(), Var(1))))SetClrModulation(RGBa(100,250,100,70),helper,1);
 else SetClrModulation(RGBa(250,100,100,70),helper,1);
}

func ShowBuidling_Closed(){
if(helper)RemoveObject(helper);
}





protected func CreateConstructionSite(idType,caller) // wird durch appendto im EPIC teil überschrieben !!!
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (!SEqual(GetAction(Contained()), "Walk")) return(nil);
  if (Contained(Contained())) return(nil);
  // Pruefen, ob das Gebaeude hier gebaut werden kann
  if (idType->~RejectConstruction(nil, 10, Contained()) ) return(nil);
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  // Baustelle erschaffen
  var pSite;
  if (!(pSite = CreateConstruction(idType, nil, 10, GetOwner(Contained()), 1, 1,1))) return(nil);
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
