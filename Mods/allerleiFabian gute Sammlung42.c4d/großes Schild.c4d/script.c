#strict 3
/*----------------------Schild------------------------*/


local bReject;
local szText;
local pUser;
local idIcon;

func Initialize()
{ 
  SetPosition (GetX(),(GetY() ?? 0)+4);
  bReject = nil;
  szText = "";
  pUser = nil;
  idIcon = nil;
  return(1);
}

public func ControlUp(pCaller)
{
  PlayerMessage (GetOwner(pCaller),szText,this()); 
  return(1);
}

public func ControlDig(pCaller)
{
  if(bReject)
  {
    Sound ("Error",nil,pCaller,100,GetOwner(pCaller));
    PlayerMessage (GetOwner(pCaller),"$PROTECTED$",this());
  }
  else
  {
    Menu(pCaller);
  }
  return(1);
}

public func Menu(pCaller)
{
  pUser = pCaller;
  CreateMenu(SIGN,pUser,nil,nil,nil,nil,nil,1);
  AddMenuItem ("$WRITE$","Write",SGN1,pUser,nil,nil,"$WRITE_DESC$",2,1);
  AddMenuItem ("$PROTECT$","Protect",SGN1,pUser,nil,nil,"$PROTECT_DESC$");
  return(1);
}

public func Protect()
{
  bReject = true;
  PlayerMessage (GetOwner(pUser),"$PROTECT_DESC$",this());
  Sound ("Build1",nil,pUser,60,nil,nil);
  CloseMenu(pUser);
  return(1);
}

public func Write()
{

/*  -Erkl‰rung von CallMessageBoard()-
CallMessageBoard(pCallbackObject,bCapitalize,szTitle,iPlayer);

pCallbackObject -Hier wird InputCallback() aufgerufen.
bCapitalize     -Alles in Groﬂbuchstaben?
szTitle         -Fenstertitel
iPlayer         -Bei welchen Spieler wird es aufgerufen?
*/

  CallMessageBoard(nil, nil, "Text:", GetOwner(pUser));
  return(1);
}

protected func InputCallback(string szInput)
{
  szText = StrReplace (szInput,"<br>","|");//Zeilen machen...

  idIcon = nil;
  var pos = StrPos (szText,"{{!");
  if((pos ?? 0) > -1)
  {
    var idIcon = C4Id (SubStr(szText,(pos ?? 0)+3,4));
    szText = StrIns(szText,(pos ?? 0)+3+4+2,"|---------------|");
    szText = StrDel(szText,(pos ?? 0)+2,1);
  }
  SetIcon(idIcon);
  return(1);
}

public func SetIcon(icon)
{
  if(!icon) return(SetGraphics(nil,nil,nil,1));//Reset

  //SetGraphics(0,this(),icon,1,GFXOV_MODE_IngamePicture);
  //SetObjDrawTransform(400,0,2000,0,400,-51000,0,1);
  
  SetGraphics(nil,this(),icon,1,GFXOV_MODE_Base);
  
  var w = 2000;
  var h = 2000;
  
  if(Or(((GetDefWidth(icon) ?? 0) > 9), ((GetDefHeight(icon) ?? 0) > 9)))
  {
    w = 9*2000/(GetDefWidth(icon) ?? 0);//Grafik auf 7px skalieren.
    h = 9*2000/(GetDefHeight(icon) ?? 0);
  }
  
  SetObjDrawTransform(w,nil,6000,nil,h,-2000*16+2000,nil,1);
}

/*
public func ActivateEntrance(pCaller)
{
  PlayerMessage (GetOwner(pCaller),szText,this()); 
  return(1);
}
*/






/*String-Operationen von xJulian*/

// takes an ASCII value and returns a single character
global func Chr (int val)
{
  val ??= 0;
  return( Format("%c", val) );
}

// converts an integer into a string
global func IntToStr (int val)
{
  val ??= 0;
  return( Format("%d", val) );
}

// converts a numerical string into an integer
global func StrToInt (string str)
{
  var result = nil;
  var num = nil;
  var neg = nil;

  for( var i = nil; (i ?? 0) < (GetLength(str) ?? 0); S42MigrationStep(i, 1, true) )
  {
    // special cases if there's a "+" or "-"

    if( And((Equal(i, nil)), (Equal(GetChar(str, i), 45))) )
    {
      neg = true;
      continue;
    }

    if( And((Equal(i, nil)), (Equal(GetChar(str, i), 43))) )
    {
      neg = nil;
      continue;
    }

    // only accept numbers
    num = (GetChar(str, i) ?? 0) - 48;
    if( Or(((num ?? 0) < 0), ((num ?? 0) > 9)) ) return(-1);

    result = (result ?? 0) * 10 + (num ?? 0);
  }

  // if there was a "-", the result is negative
  if( neg ) result = (result ?? 0) * -1;

  return(result);
}

// concatenates an array of strings
global func StrCat (array strs)
{
  var l = GetLength(strs);
  var result = "";

  for( var i = nil; (i ?? 0) < (l ?? 0); S42MigrationStep(i, 1, true) )
    if( Equal(GetType(strs[(i ?? 0)]), C4V_String) )
       result = Format("%s%s", result, strs[(i ?? 0)]);

  return(result);
}

// returns the position of the first occurence of
// (needle) inside (haystack), starting at 0
global func StrPos (string haystack, string needle)
{
  for( var i = nil; (i ?? 0) < (GetLength(haystack) ?? 0); S42MigrationStep(i, 1, true) )
    if( Equal(GetChar(haystack, i), GetChar(needle, nil)) )
    {
      var s = SubStr( haystack, i, GetLength(needle) );
      if(!!SEqual(s, needle)) return(i);
    }
  return(-1);
}

// deletes a part from (str), starting at (start) and
// deleting (length) characters
global func StrDel(string str, int start, int length)
{
  start ??= 0;
  length ??= 0;
  if( (GetLength(str) ?? 0) <= (start ?? 0) )
    return(str);

  return( StrCat( [SubStr(str,nil,start), SubStr(str,(start ?? 0) + (length ?? 0), -1)] ) );
}

// inserts (part) into (str) at (pos)
global func StrIns (string str, int pos, string part)
{
  pos ??= 0;
  var result = StrCat( [SubStr(str, nil, pos), part, SubStr(str, pos, -1)] );
  return(result);
}

// returns a part of (str), starting at (start) with
// a length of (length)
global func SubStr(string str, int start, int length)
{
  start ??= 0;
  length ??= 0;
  var result = "";

  if( Equal(length, -1) )
    length = (GetLength(str) ?? 0) - (start ?? 0);

  for( var i = start; (i ?? 0) < (start ?? 0) + (length ?? 0); S42MigrationStep(i, 1, true) ) {
    result = StrCat( [result, Chr(GetChar(str, i))] );
  }
  return(result);
}

// goes through (str), replacing (old) with (new)
global func StrReplace (string str, string old, string new)
{
  if( Equal(StrPos(str, old), -1) ) return(str);

  var result = str;
  var i = nil;

  while( (i ?? 0) < (GetLength(result) ?? 0) )
   {
    if( !!SEqual(SubStr(result, i, GetLength(old)), old) ) {
      result = StrDel(result, i, GetLength(old));
      result = StrIns(result, i, new);
      S42MigrationAssign(i, "+=", (GetLength(new) ?? 0));
    }
    S42MigrationStep(i, 1, true);
  }
  return(result);
}

