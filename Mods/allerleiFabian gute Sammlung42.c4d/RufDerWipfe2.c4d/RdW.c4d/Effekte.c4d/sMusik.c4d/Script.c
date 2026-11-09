#strict

static nomusic;
local krieg;

protected func Initialize()
{
  krieg=0;
  SetAction("Initialize");
  return(1);
}

protected func Initialized()
{
  CheckPlayList();
  return(1);
}

protected func Dawn()
{
   CheckPlayList();
}

protected func UpdateTransferZone()
{
  CheckPlayList();
  return(1);
}


public func CheckPlayList()
{
  if(nomusic)return 0;
	
  var trackA,trackB,obj;
  /* Jahreszeit */
  if(GetTemperature()<=0) 	trackA="Winter*";
  else 				trackA="Sommer*";

  /* Tageszeit */
  if(IsDay()) 	trackB="*Tag*";
  else 		trackB="*Nacht*";
  
  if(Krieg()){trackA="Krieg*"; trackB="";}
  
  if(GetPlayerCount()==1)
   if(obj=FindObject(_GLZ))
    if(obj->~PbIstGolden(GetPlayerByIndex()))
     trackA="Golden*";

  if(SetPlayList(Format("*%s%s*",trackA,trackB))+IsNetwork())
    return 1;
  else
  {
    if(SetPlayList(Format("*%s*",trackA)))
      return 2;
    else if(SetPlayList(Format("*%s*",trackB)))
      return 3;
    else if(SetPlayList())
      return 4;
  }
}

global func Krieg()
{
  var spieler=0;
  for(var i=0; spieler<GetPlayerCount(); i++)
  {
  	if(GetPlayerName(i)) spieler++;
  	for(var j=0; j<i; j++)
  	{
  		if(Hostile(i,j))
  			return(1);
  	}
  }
  return(0);
}

global func DeactivateAutoMusic()
{
  nomusic=1;
  SetPlayList(0);
}

global func ActivateAutoMusic()
{
  nomusic=0;
  if(FindObject(SMUS))FindObject(SMUS)->CheckPlayList();
}
