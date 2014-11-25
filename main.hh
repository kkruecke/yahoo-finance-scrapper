<?hh
use Yahoo\CSVWriter;
use Yahoo\TableRowExtractorIterator;

require_once("loader/SplClassLoader.php");
require_once("utility.hh");

$spl_loader = new SplClassLoader('Yahoo', 'src/');

$spl_loader->register();

$registry = new Registry(); // Work around to get class autoloaed.

  if ($argc == 2) {

    $argv[2] = 0; 
    $argc = 3;
  }

  $error_msg = '';

  if (validate_user_input($argc, $argv, $error_msg) == false) {

       echo $error_msg . "\n";

       echo Registry::registry('help'); 
       return;
  }

  $date_period = build_date_period($argv[1], (int) $argv[2]);
  
  $csv_writer = new CSVWriter(new CSVYahooFormatter(),  $start_date, $argv[2]);

  // Start main loop
  foreach ($date_period as $date_time) {
      
      // Build yyyymmdd.html name

      $url = make_url($date_time);
      $pretty_date = $date_time->format("m-d-Y");
      
      if (validate_url_existence($url)) {
          
           echo 'Skipping date ' . $pretty_date . " there is no webpage $url ...\n";               
           continue;    
      }
      
      try {
          
	  $table = new YahooTable($url, Registry::registry('xpath-query'));
	  $max_row = $table->rowCount();
	     
	  // We shik the first two rows, the table description and column headers.
	  $limitIter = new \LimitIterator($table->getIterator(), 2, $max_rows - 1); // TODO: ...or is it "- 2"?

	  /*
	   * The filter iterator should include all the filters of the original code:
	   *   1. no column may be blank
	   *   2. only US Stocks are selected
	   *   3. ? any other filters
	   */   
	  $filterIter = new \CustomStockFilterIterator($limitIter);

          //$callbackFilterIter = new \CallbackFilterIterator($rowExtractorIter, 'isUSStock_callback');
     
          //foreach($callbackFilterIter as $us_stock_row) {
          foreach($filterIter as $us_stock_row) {
               // TODO:
  	       // process the $us_stock_row
               $csv_writer->writeLine($us_stock_data); 
	  }

	  echo "Date $pretty_date processed\n";

  
      } catch(Exception $e) {
          
          echo $e->getMessage() . "\n";
          return;
      }
  }

  $us_stock_count = $csv_writer->getLineCount();
  
  echo "A total of " . $us_stock_count . " US stocks were extracted.\n";
  echo  $csv_writer->getFileName() . " has been created. It contains $us_stock_count US stocks entries.\n";
    
  return;
