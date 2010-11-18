for $country in doc("places_countryinfo.xml")//geonames/country
return(
if(data($country/countryCode)=$countryCodeToSearch)
then ( ($country) )
else()
)
