#strict

local clonk;

func Activate(pByObj) {
clonk=pByObj;
  var find;
var playername=GetPlayerName(GetOwner());

//ein paar IDs von Gebäuden ohne Eingang, die aber trzd. abgeshclossen werden müssen
if (find=FindObject2(Find_Or(Find_OCF(OCF_Entrance),Find_ID(SLTW),Find_ID(CGDT),Find_ID(CDFT)),Find_AtRect(-5,-5,10,20),Find_NoContainer() ) )
{                

      if (GetOwner()==GetOwner(find)) 
      Verwenden(GetID(find),find);

            if (GetOwner()!=GetOwner(find)) 
            PlayerMessage(GetOwner(clonk),Format("Der Schlüssel gehört %s und passt nur dort",playername),clonk);
}
else {PlayerMessage(GetOwner(clonk),"Nichts zum Ab-/Aufschließen",clonk);}
 
}



func Verwenden(ID, Objekt)
{

        if(LocalN("zuu",Objekt)==0) {
          LocalN("zuu",Objekt)=1;
          PlayerMessage(GetOwner(clonk),Format("%v abgeschlossen",GetName(Objekt)),clonk);
          //Log("abgeschlossen");
        } 
          else {
          LocalN("zuu",Objekt)=0;
          PlayerMessage(GetOwner(clonk),Format("%v aufgeschlossen",GetName(Objekt)),clonk); 
          //Log("aufgeschlossen");    
          }
}

public func IsAnvilProduct() { return(1); }




