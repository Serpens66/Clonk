/*-- KI-Händler erforschen --*/

#strict 3

func GetResearchTime() { return 1;}
func GetResearchCosts() { return 100;}



// je nach Regelwahl ist dies entweder erforschbar, oder nicht.
public func GetResearchBase33() { 
if(FindObject2(Find_ID(ERI6))) return(ERIT);
else return ;
}

