#strict 3



global func PathFree (int iX1, int iY1, int iX2, int iY2,fahrstuhl)
{
iX1 ??= 0;
iY1 ??= 0;
iX2 ??= 0;
iY2 ??= 0;
var ok; 
if(fahrstuhl) // wird im ELEC fahrstuhlkorb mit übergeben und ist das Fahrstuhlkorbobjekt
    {
    var obje;

    while(obje=FindObject(SSPE,nil,nil,-1,-1,nil,"Closed",nil,nil,obje))
        {
        obje->RemoveMask();
        }
    ok=inherited(iX1,iY1,iX2,iY2,fahrstuhl);
    while(obje=FindObject(SSPE,nil,nil,-1,-1,nil,"Closed",nil,nil,obje))
        {
        obje->SetMask();
        }
    return(ok);
    }
return(inherited(iX1,iY1,iX2,iY2,fahrstuhl));
}
