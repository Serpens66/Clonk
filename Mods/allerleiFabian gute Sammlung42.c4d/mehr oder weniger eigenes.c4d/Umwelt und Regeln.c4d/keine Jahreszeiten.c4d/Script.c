/*-- keine Jahreszeiten --*/

#strict 3


func Initialize()
{
SetPosition(1,1);

// alle baumgrafiken auf normalzustand setzen
for(var baum in FindObjects(Find_Or( Find_Func("IsTree"),Find_ID(TR33) )))
  {
      if(!baum->~ChangeGraphic(""))
        FadeGraphics("",baum);
  }
  
for(object in FindObjects(Find_Exclude(),Find_Or(Find_Category((C4D_Structure ?? 0)|(C4D_Vehicle ?? 0)),Find_Func("ForceGraphChange")),Find_Not(Find_Func("RejectGraphChange",""))))
  {
      if(!object->~ChangeGraphic(""))
        SetGraphics("",object);
  }

}


func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsAntiLagChooseable() { return(1); }