<?php

	function Connection(){
		$server="files.000webhost.com"; //localhost
		$user="treecanopy"; //treecanopy
		$pass="treecanopy"; //treecanopy
		$db="database";
	   	
		$connection = mysql_connect($server, $user, $pass);

		if (!$connection) {
	    	die('MySQL ERROR: ' . mysql_error());
		}
		
		mysql_select_db($db) or die( 'MySQL ERROR: '. mysql_error() );

		return $connection;
	}
?>
