/*--- AppendToMaterials ---*/

#strict
#appendto ASHS
#appendto CNCR
#appendto ERTH
#appendto ICE1
#appendto LIQG
#appendto LOAM
#appendto SAND
#appendto SNWB

func MaterialMange() {
if(GetID() == ASHS ){return(200);} 
if(GetID() == CNCR ){return(100);} 
if(GetID() == ERTH ){return(200);} 
if(GetID() == ICE1 ){return(400);} 
if(GetID() == LIQG ){return(150);} 
if(GetID() == LOAM ){return(400);} 
if(GetID() == SAND ){return(200);} 
if(GetID() == SNWB ){return(400);} 
}

func MaterialNameString() { 
if(GetID() == ASHS ){return("Ashes"); }
if(GetID() == CNCR ){return("Rock");}
if(GetID() == ERTH ){return("Earth");}
if(GetID() == ICE1 ){return("Ice");}
if(GetID() == LIQG ){return("Granite");}
if(GetID() == LOAM ){return("Earth");}
if(GetID() == SAND ){
if(Random(10)){ return("FlySand");} 
return("Sand");}
if(GetID() == SNWB ){return("Snow");}
}
