#strict
/*----------------------Schild------------------------*/


local bReject;
local szText;
local pUser;
local idIcon;

func Initialize()
{ 
  SetPosition (GetX(),GetY()+2); 
  bReject = false;
  szText = "";
  pUser = 0;
  idIcon = 0;
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
    Sound ("Error",false,pCaller,100,GetOwner(pCaller)); 
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
  CreateMenu(SIGN,pUser,0,0,0,0,0,1);
  AddMenuItem ("$WRITE$","Write",SGN1,pUser,0,0,"$WRITE_DESC$",2,1);
  AddMenuItem ("$PROTECT$","Protect",SGN1,pUser,0,0,"$PROTECT_DESC$");
  return(1);
}

public func Protect()
{
  bReject = true;
  PlayerMessage (GetOwner(pUser),"$PROTECT_DESC$",this());
  Sound ("Build1",false,pUser,60,0,0);
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

  CallMessageBoard(0, false, "Text:", GetOwner(pUser));
  return(1);
}

protected func InputCallback(string szInput)
{
  szText = StrReplace (szInput,"<br>","|");//Zeilen machen...

  idIcon = 0;
  var pos = StrPos (szText,"{{!");
  if(pos > -1)
  {
    var idIcon = C4Id (SubStr(szText,pos+3,4));
    szText = StrIns(szText,pos+3+4+2,"|---------------|");
    szText = StrDel(szText,pos+2,1);
  }
  SetIcon(idIcon);
  return(1);
}

public func SetIcon(icon)
{
  if(!icon) return(SetGraphics(0,0,0,1));//Reset

  //SetGraphics(0,this(),icon,1,GFXOV_MODE_IngamePicture);
  //SetObjDrawTransform(400,0,2000,0,400,-51000,0,1);
  
  SetGraphics(0,this(),icon,1,GFXOV_MODE_Base);
  
  var w = 1000;
  var h = 1000;
  
  if((GetDefWidth(icon) > 9)||(GetDefHeight(icon) > 9))
  {
    w = 9*1000/GetDefWidth(icon);//Grafik auf 7px skalieren.
    h = 9*1000/GetDefHeight(icon);
  }
  
  SetObjDrawTransform(w,0,2000,0,h,-1000*16,0,1);
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
  return( Format("%c", val) );
}

// converts an integer into a string
global func IntToStr (int val)
{
  return( Format("%d", val) );
}

// converts a numerical string into an integer
global func StrToInt (string str)
{
  var result = 0;
  var num = 0;
  var neg = false;

  for( var i = 0; i < GetLength(str); i++ )
  {
    // special cases if there's a "+" or "-"

    if( (i == 0) && (GetChar(str, i) == 45) )
    {
      neg = true;
      continue;
    }

    if( (i == 0) && (GetChar(str, i) == 43) )
    {
      neg = false;
      continue;
    }

    // only accept numbers
    num = GetChar(str, i) - 48;
    if( (num < 0) || (num > 9) ) return(-1);

    result = result * 10 + num;
  }

  // if there was a "-", the result is negative
  if( neg ) result = result * -1;

  return(result);
}

// concatenates an array of strings
global func StrCat (array strs)
{
  var l = GetLength(strs);
  var result = "";

  for( var i = 0; i < l; i++ )
    if( GetType(strs[i]) == C4V_String )
       result = Format("%s%s", result, strs[i]);

  return(result);
}

// returns the position of the first occurence of
// (needle) inside (haystack), starting at 0
global func StrPos (string haystack, string needle)
{
  for( var i = 0; i < GetLength(haystack); i++ )
    if( GetChar(haystack, i) == GetChar(needle, 0) )
    {
      var s = SubStr( haystack, i, GetLength(needle) );
      if(s eq needle) return(i);
    }
  return(-1);
}

// deletes a part from (str), starting at (start) and
// deleting (length) characters
global func StrDel(string str, int start, int length)
{
  if( GetLength(str) <= start )
    return(str);

  return( StrCat( [SubStr(str,0,start), SubStr(str,start + length, -1)] ) );
}

// inserts (part) into (str) at (pos)
global func StrIns (string str, int pos, string part)
{
  var result = StrCat( [SubStr(str, 0, pos), part, SubStr(str, pos, -1)] );
  return(result);
}

// returns a part of (str), starting at (start) with
// a length of (length)
global func SubStr(string str, int start, int length)
{
  var result = "";

  if( length == -1 )
    length = GetLength(str) - start;

  for( var i = start; i < start + length; i++ ) {
    result = StrCat( [result, Chr(GetChar(str, i))] );
  }
  return(result);
}

// goes through (str), replacing (old) with (new)
global func StrReplace (string str, string old, string new)
{
  if( StrPos(str, old) == -1 ) return(str);

  var result = str;
  var i = 0;

  while( i < GetLength(result) )
   {
    if( SubStr(result, i, GetLength(old)) eq old ) {
      result = StrDel(result, i, GetLength(old));
      result = StrIns(result, i, new);
      i += GetLength(new);
    }
    i++;
  }
  return(result);
}

