/* Zusatzfunktionen */
#strict
//static const GraphFadeLayer=-1; // Erstmal 1 bis Background-Layer implentiert ist
static const GraphFadeLayer=1;

global func MessageWindow(string pMsg, int iForPlr, id idIcon, string pCaption)
{
  // get icon
  if (!idIcon) idIcon=GetID();
  // get caption
  if (!pCaption) pCaption=GetName();
  // create msg window (menu)
  var pCursor = GetCursor(iForPlr);
  if (!CreateMenu(idIcon, pCursor, pCursor, 0, pCaption, 0, 2)) return();
  AddMenuItem(pCaption, "", idIcon, pCursor,0,0,pMsg);
  return(1);
}

global func SIcon(idDef,i,dwColor)
{
  if(dwColor)
    return Format("<c %x>{{%i:%d}}</c>",dwColor,idDef,i);
  return Format("{{%i:%d}}",idDef,i);
}

/* ---------- */

global func GetSign(iVal)
{
  if(iVal>0)
   return 1;
  if(iVal<0)
   return -1;
  return 0;
}

global func SinX(iA,iMin,iMax) { return(Sin(iA,(iMax-iMin)/2)+(iMax+iMin)/2); }
global func CosX(iA,iMin,iMax) { return(Cos(iA,(iMax-iMin)/2)+(iMax+iMin)/2); }

global func Rotate(Rot,Obj)
{
  if(!Obj)
   if(!(Obj=this()))
    return 0;
  if(!Rot)
   return RemoveEffect("Rotate",Obj,0);
  return AddEffect("Rotate",Obj,110,1,0,0,Rot);
}

/* ----- Effekt: Rotate ----- */

global func FxRotateStart(object Target, int Effectnumber, int Temp, Speed)
{
  EffectVar(1,Target,Effectnumber)=Speed;
  return 0;
}

global func FxRotateTimer(object Target, int Effectnumber, int EffectTime)
{
  var speed = EffectVar(1,Target,Effectnumber);
  SetR(GetR(Target)+speed,Target);
  return 1;
}

global func FxRotateEffect(string NewEffectName, object Target, int EffectNumber)
{
  if(NewEffectName S= "Rotate")
   return -2;
  return 0;
}

global func FxRotateAdd(object Target, int Effectnumber, string NewEffectName, int NewEffectTimer, Speed)
{
  EffectVar(1,Target,Effectnumber)=Speed;
  ChangeEffect(0,Target,Effectnumber,0,NewEffectTimer);
  return 0;
}

/* ----- ENDE:Rotate ----- */

global func AttachTo(Obj,Obj2)
{
  if(!Obj2)
   if(!(Obj2=this()))
    return 0;
  if(!Obj)
   return RemoveEffect("Attach",Obj2,0);
  return AddEffect("Attach",Obj2,110,1,0,0,Obj);
}

/* ----- Effekt: Attach ----- */

global func FxAttachStart(object Target, int Effectnumber, int Temp, Object)
{
  EffectVar(1,Target,Effectnumber)=Object;
  return 0;
}

global func FxAttachTimer(object Target, int Effectnumber, int EffectTime)
{
  var obj = EffectVar(1,Target,Effectnumber);
  SetPosition(GetX(obj),GetY(obj),Target);
  return 1;
}

global func FxAttachEffect(string NewEffectName, object Target, int EffectNumber)
{
  if(NewEffectName S= "Attach")
   return -2;
  return 0;
}

global func FxAttachAdd(object Target, int Effectnumber, string NewEffectName, int NewEffectTimer, NewObject)
{
  EffectVar(1,Target,Effectnumber)=NewObject;
  ChangeEffect(0,Target,Effectnumber,0,NewEffectTimer);
  return 0;
}

/* ----- ENDE:Attach ----- */

global func FadeGraphics(sGraphic,pObject,idDef,fIgnoreSame)
{
  if(!fIgnoreSame)
    if(GetGraphics(pObject) S= sGraphic)
      return 2;
  if(AddEffect("graphFade",pObject,100,RandomX(3,5),0,0,sGraphic,idDef,GraphFadeLayer))
    return(1);
  //Log("ERROR:FadeGraphics");
  return(0);
  //return(SetGraphics(sGraphic,pObject,idDef));
}

global func SetObjAlpha(byAlpha, pObj,iLayer)
{
  var byAlpha=BoundBy(byAlpha,0,255);
  var dwClrMod=GetClrModulation(pObj,iLayer);
  if(!dwClrMod)
    dwClrMod=RGB(255,255,255);
    dwClrMod=SetRGBaValue(dwClrMod,byAlpha);
  if(iLayer)
    return(SetClrModulation(dwClrMod, pObj,iLayer));
  else
    return(SetClrModulation(dwClrMod, pObj));
}

/* ----- Effekt: graphFade ----- */

global func FxgraphFadeStart(object pObject, int iEffectNumber, Temp, sGraphic, idDef , iLayer)
{
  if(Temp)
    return(0);
  if(!idDef)
    idDef=GetID(pObject);
  SetGraphics(0,pObject,0,iLayer);
  if(!SetGraphics(sGraphic,pObject,idDef,iLayer,GFXOV_MODE_ExtraGraphics))
  {
    //Log("ERROR:FxgraphFadeStart");
    return(-1);
  }
  EffectVar(1,pObject,iEffectNumber)=iLayer;
  EffectVar(2,pObject,iEffectNumber)=sGraphic;
  EffectVar(3,pObject,iEffectNumber)=idDef;
  SetObjAlpha(255, pObject,iLayer);
  return(1);
}

global func FxgraphFadeTimer(object pObject, int iEffectnumber, int iEffectTime)
{
  var iLayer=EffectVar(1,pObject,iEffectnumber);
  var AlphaA=BoundBy(GetRGBaValue(GetClrModulation(pObject,iLayer))-(4*(1+IsAntiLag())),1,255);
  SetObjAlpha(AlphaA,pObject,iLayer);
  //var AlphaB=BoundBy(GetRGBaValue(GetClrModulation(pObject))+1,1,255);
  if(AlphaA < 127)
  {
    var AlphaB=BoundBy(GetRGBaValue(GetClrModulation(pObject))+(5*(1+IsAntiLag())),1,255);
    SetObjAlpha(AlphaB,pObject);
  }
  if(AlphaB == 255 && AlphaA == 1)
  {
    return(-1);
  }
  return(1);
}

global func FxgraphFadeStop(object pObject, int iEffectNumber, iReason, Temp)
{
  //Log("FxgraphFadeStop");
  if(Temp)
    return(0);
  var iLayer=EffectVar(1,pObject,iEffectNumber);
  var sGraphic=EffectVar(2,pObject,iEffectNumber);
  var idDef=EffectVar(3,pObject,iEffectNumber);
  SetGraphics(sGraphic,pObject,idDef);
  SetGraphics(0,pObject,0,iLayer);// Den Effektlayer löschen...
  SetObjAlpha(0, pObject);
  return(1);
}

global func FxgraphFadeEffect(string szNewEffectName)
{
  if(szNewEffectName S= "graphFade")
   return(-1);
  return(0);
}

//____::::____

//
global func SetLayerTransform(iLayer,iR,iXCon,iYCon,iXPos,iYPos,pTarget)
{
  
  SetObjDrawTransform(iXCon*10, 0, 1000*iXPos,
                      0, iYCon*10, 1000*iYPos,
                      pTarget, iLayer);
  // *sniff* funktionukelt leider nicht... :( (Bei Drehung wird die grösse falsch berechnet)
  //var fsin=-Sin(iR, 1000), fcos=Cos(iR, 1000);
  /*SetObjDrawTransform
    (
        iXCon*10+fcos-1000,
        0+fsin,
        (1000-fcos)*iXPos - fsin*iYPos,
        0-fsin,
        iYCon*10+fcos-1000,
        (1000-fcos)*iYPos + fsin*iXPos,
        pTarget, iLayer
    );*/
  return 1;
}

global func Prozent(voll,anteil) // gibt P1 Prozent von P0 zurück
{  
	return(voll*anteil/100);
}

global func ProzentVon(voll,anteil) // wieviel Prozent von P0 sind P1?
{
  return(100*anteil/voll);
}

global func SetActIdle(){ return(SetAction("Idle"));}

global func GetPlrCount(){ return(GetPlayerCount());}

global func EnergyCount(idDef,actname)
{
 	var objekt,zahl=0;
	while(objekt=FindObject(idDef,0,0,0,0,0,actname,0,0,objekt))
	 if(EnergyCheck(1000,objekt))
	  zahl++;
	return(zahl);
}

global func FriendlyClonk(clonk) // für Kontext-Conditions
{
	return(!Hostile(GetOwner(clonk),GetOwner()));
}	

global func Find_Hostile(int plr,int ePlayerType) {
	var p = [C4FO_Or];
	for(var i = -1; i < GetPlayerCount(ePlayerType); i++)
		if(Hostile(plr, GetPlayerByIndex(i,ePlayerType)))
			p[GetLength(p)] = Find_Owner(GetPlayerByIndex(i,ePlayerType));
	return(p);
}