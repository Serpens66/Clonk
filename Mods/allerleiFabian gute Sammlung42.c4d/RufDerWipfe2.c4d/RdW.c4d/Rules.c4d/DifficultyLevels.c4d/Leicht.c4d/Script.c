#strict 3

func Activate(iPlr)
{
  return(MessageWindow(GetDesc(),iPlr));
}

public func GetFood()	{return(1);}
public func GetScienceCost()  {return(15);}
public func GetTechLevel()  {return(3);}
public func GetRevTime()   {return(1);}
public func GetPollutionPerc()  {return(25);}

public func IsDifficultyLevel() {return 1;}
public func Color(){return RGB(127,200,255);}
