/*-- Kristallmine --*/

#strict 3
#include GGMI

private func MatObjsCount(...) {  return(ObjectCount(CRYS));
}
private func MatPixelCount(...) { return(GetMaterialCount(Material("Crystal")));
}
private func MatName(...) {       return("Kristall");
}

