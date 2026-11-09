#strict 3

static nomusic;
local krieg;

protected func Initialize()
{
  krieg=nil;
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
  if(nomusic)return nil;
	
  var trackA,trackB,obj;
  /* Jahreszeit */
  if((GetTemperature() ?? 0)<=0) 	trackA="Winter*";
  else 				trackA="Sommer*";

  /* Tageszeit */
  if(IsDay()) 	trackB="*Tag*";
  else 		trackB="*Nacht*";
  
  if(Krieg()){trackA="Krieg*"; trackB="";}
  
  if(Equal(GetPlayerCount(), 1))
   if(obj=FindObject(_GLZ))
    if(obj->~PbIstGolden(GetPlayerByIndex()))
     trackA="Golden*";

  if((SetPlayList(Format("*%s%s*",trackA,trackB)) ?? 0)+(IsNetwork() ?? 0))
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
  var spieler=nil;
  for(var i=nil; (spieler ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
        if(GetPlayerName(i)) S42MigrationStep(spieler, 1, true);
        for(var j=nil; (j ?? 0)<(i ?? 0); S42MigrationStep(j, 1, true))
  	{
  		if(Hostile(i,j))
  			return(1);
  	}
  }
  return(nil);
}

global func DeactivateAutoMusic()
{
  nomusic=1;
  SetPlayList(nil);
}

global func ActivateAutoMusic()
{
  nomusic=nil;
  if(FindObject(SMUS))FindObject(SMUS)->CheckPlayList();
}
