This is a data scrapper for Yahoo financial data. It is configuration-driven by an .ini file that specifies the

* The base url of the Yahoo financial data
* the xpath query used to return the HTML table with the data

The date entered, passed on the command line, is used to construct the specific url path that is then appended to the base url. 

**YahooTable** holds the table with all the financial data. Use the constructor to specify the start and end column that you want its external iterator **YaooTableIterator**
to return.  To fruther limit the range of rows of the iteration, pass **YahooTableIterator** to a **LimitIterator**, for example:

	  // To skip the first two rows, the table description and column headers, as well as the last row, use a LimitIterator.
	  $limitIter = new \LimitIterator($table->getIterator(), 2, $max_rows - 1); // TODO: ...or is it "- 2"?

 To further filter the rows returned, extend **FilterIterator** and pass it either a **YahooTableIterator** instance or (as explained above) a **LimitIterator**.
