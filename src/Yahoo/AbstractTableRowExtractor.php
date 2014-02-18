<?php
namespace Yahoo;

// Should it implement Iterator? I don't think so.
abstract class AbstractTableRowExtractor implements \Iterator {
  
   protected $trdNodesList;
 
  /*
   *  This is what should be passed as $xpath_query
   * '/html/body/table[3]/tr/td[1]/table[1]'
   *
   */
  
  public function __construct($url, $xpath_table_query)
  {
     $page = file_get_contents($url);
     
    //Debug:- file_put_contents("./$html_file_name", $page); // Debug only
    
     // a new dom object
    $this->dom = new \DOMDocument;
     
    // load the html into the object
    $this->dom->strictErrorChecking = false; // default is true.

    @$this->dom->loadHTML($page);  // Turn off error reporting
     
    // discard redundant white space
    $this->dom->preserveWhiteSpace = false;
    
    $this->xpath = new \DOMXPath($this->dom);
    
    // returns nodelist -- must first get the first and only node, the table.
    // 
    $xpathNodeList = $this->xpath->query($xpath_table_query);
    
    if ($xpathNodeList->length != 1) { // TODO: Change to throw Exception
        
        throw new Exception("XPath Query\n $xpath_table_query\n   Failed. Page format has evidently changed. Cannot proceed.\n");
    } 
 
    $tableNodeElement = $xpathNodeList->item(0);
    
    /* 
     * We need to as the $tableNodeElement->length to get the number of rows. We will subtract the first two rows --
     * the "Earnings Announcement ..." and the columns headers, and we ignore the last row.
     * Query Paths for the rows:
     * 1.  /html/body/table[3]/tr/td[1]/table[1]/tr[1] is "Earnings Announcements for Wednesday, May 15"
     * 2.  /html/body/table[3]/tr/td[1]/table[1]/tr[2] is column headers
     */
    
     if (!$tableNodeElement->hasChildNodes()) {
         
        throw new Exception("This is no table element at \n $xpath_table_query\n. Page format has evidently changed. Cannot proceed.\n");

     } else {

        // Seems ok 
        $this->trNodesList = $tableNodeElement->childNodes;
     }  

  } // end __construct()
  

  // code simply copy from getyahoo.php
  // Does it belong it here. See question below about whether getRowData() implies that this class is a table row iterator? Maybe I should make such an iterator?
  abstract protected function getRowData($id);
 
} // end class
?>
