/*-- Statue --*/

#strict 3
#include IDOL


/*
  Die verschiedenen Aktionen des Sensor-Ziels.
*/
public func GetTriggerName(int a)
{
	a ??= 0;
	if(Equal(a, 1))//Pink färben.
	{
		return("PINK");
	}
  
	if(Equal(a, 2))//Entfärben.
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
	if(Equal(a, 1))//Pink färben.
	{
		SetClrModulation (RGBa(255,nil,128,nil),this());
	}
  
	if(Equal(a, 2))//Entfärben.
	{
		SetClrModulation (RGBa(nil,nil,nil,nil),this());
	}
  
  return(true);	
}