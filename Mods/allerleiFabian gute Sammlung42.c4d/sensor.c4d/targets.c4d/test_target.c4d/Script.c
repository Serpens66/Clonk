/*-- Statue --*/

#strict
#include IDOL


/*
  Die verschiedenen Aktionen des Sensor-Ziels.
*/
public func GetTriggerName(int a)
{
	if(a==1)//Pink färben.
	{
		return("PINK");
	}
  
	if(a==2)//Entfärben.
	{
		return("NORMAL");
	}
  
  return(true);//Muss immer TRUE zurückgeben!
}

/*
  Hier werden die Aktionen ausgelöst.
*/
public func Trigger(a)
{
	if(a==1)//Pink färben.
	{
		SetClrModulation (RGBa(255,0,128,0),this());
	}
  
	if(a==2)//Entfärben.
	{
		SetClrModulation (RGBa(0,0,0,0),this());
	}
  
  return(true);	
}