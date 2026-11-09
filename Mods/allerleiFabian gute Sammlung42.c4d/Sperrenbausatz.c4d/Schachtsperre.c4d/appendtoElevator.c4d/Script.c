#strict



global func PathFree (int iX1, int iY1, int iX2, int iY2,fahrstuhl)
{
var ok; 
if(fahrstuhl) // wird im ELEC fahrstuhlkorb mit übergeben und ist das Fahrstuhlkorbobjekt
    {
    var obje;

    while(obje=FindObject(SSPE,0,0,-1,-1,0,"Closed",0,0,obje))
        {
        obje->RemoveMask();
        }
    ok=inherited(iX1,iY1,iX2,iY2,fahrstuhl);
    while(obje=FindObject(SSPE,0,0,-1,-1,0,"Closed",0,0,obje))
        {
        obje->SetMask();
        }
    return(ok);
    }
return(inherited(iX1,iY1,iX2,iY2,fahrstuhl));
}
