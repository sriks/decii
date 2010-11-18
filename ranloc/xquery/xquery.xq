for $xquery in doc("xqueries.xml")//xqueries/xquery
return(
if(data($xquery/name)=$nameToSearch)
then ( data($xquery/query) )
else())
