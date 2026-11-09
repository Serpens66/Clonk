#strict
local iMinute,iSecound,TimeOver,Disabled,Count;
func Initialize() 
{
	TimeOver = 0;
	SetVisibility(VIS_All());
  	SetPosition(50, 100, this());
  	return();
}

func Timer() 
{
	if(Count == 0) Count = -1;
	if(!Disabled) 
	{
		iSecound+= Count;
		Hexal();
		if(Count < 0) Message("<c ff0000>%d:%3.2d</c>", this(),iMinute,iSecound);
		if(iMinute < 60) if(Count > 0) Message("<c 00ff00>%d:%3.2d</c>", this(),iMinute,iSecound);
		if(iMinute > 59) if(Count > 0) Message("<c cc00ff>Süchtig!</c>",this());
	}
}

global func ChangeTimerCount(int iC)
{
	CheckHelp();
	var pTimer;
	pTimer = FindObject(TIMR);
	if(!pTimer) pTimer = CreateObject(TIMR);
	LocalN("Count",pTimer) = iC;
}

func Hexal()
{
	if(iSecound < 0 || iSecound > 59)
	{
		iMinute += Count;
		if(Count > 0) iSecound = 0;
		if(Count < 0) iSecound = 60;
	}
	if(iMinute < 0)
	{
		if(Count < 0) Down();
		iMinute = 0;
	}
	return();
}
	
func Enable()
{
	Disabled = 0;
}

global func SetTime(int iMinutes,int iSecounds)
{
	CheckHelp();
	var pTimer;
	pTimer = FindObject(TIMR);
	if(!pTimer) pTimer = CreateObject(TIMR);
	LocalN("iMinute",pTimer) = iMinutes;
	LocalN("iSecound",pTimer) = iSecounds;
	pTimer -> Enable();
	return(1);
}

func Down()
{
	Disabled = 1;
	GameCallEx("TimerDown", this());
}

global func CheckHelp()
{
	if(!FindObject(HELP)) CreateObject(HELP);
}