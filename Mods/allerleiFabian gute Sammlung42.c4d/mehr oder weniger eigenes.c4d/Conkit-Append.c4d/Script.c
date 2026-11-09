

#strict 3

/*--- Bausatz ---
#appendto CNKT

local helper,clonksize;

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  // Clonk anhalten
  SetComDir(COMD_Stop(), pClonk);
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoconstructionplansa$");
  var idType; var i = 0;
  helper=CreateObject(A_CN,0,0,GetOwner(pClonk));
  SetVisibility(VIS_Owner(),helper);
  clonksize=GetDefCoreVal("Offset",0,GetID(pClonk),1);
  var smec=CreateObject(SMEC);
  smec->Init(pClonk,this(), "ShowBuidling", 1);
  while (idType = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_Structure()))
    {AddMenuItem("$TxtConstructions$", "CreateConstructionSite2", idType, pClonk);
    smec->AddInfo(idType);
    }

	return(1);
}

protected func CreateConstructionSite2(idType)
{
ShowBuidling_Closed();
ProtectedCall(this(),"CreateConstructionSite",idType);
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
*/