/*--- AppendToMaterials ---*/

#strict 3
#appendto ASHS
#appendto CNCR
#appendto ERTH
#appendto ICE1
#appendto LIQG
#appendto LOAM
#appendto SAND
#appendto SNWB

func MaterialMange() {
if(Equal(GetID(), ASHS) ){return(200);}
if(Equal(GetID(), CNCR) ){return(100);}
if(Equal(GetID(), ERTH) ){return(200);}
if(Equal(GetID(), ICE1) ){return(400);}
if(Equal(GetID(), LIQG) ){return(150);}
if(Equal(GetID(), LOAM) ){return(400);}
if(Equal(GetID(), SAND) ){return(200);}
if(Equal(GetID(), SNWB) ){return(400);}
}

func MaterialNameString() { 
if(Equal(GetID(), ASHS) ){return("Ashes"); }
if(Equal(GetID(), CNCR) ){return("Rock");}
if(Equal(GetID(), ERTH) ){return("Earth");}
if(Equal(GetID(), ICE1) ){return("Ice");}
if(Equal(GetID(), LIQG) ){return("Granite");}
if(Equal(GetID(), LOAM) ){return("Earth");}
if(Equal(GetID(), SAND) ){
if(Random(10)){ return("FlySand");} 
return("Sand");}
if(Equal(GetID(), SNWB) ){return("Snow");}
}
