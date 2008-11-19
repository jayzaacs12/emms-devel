<?php
class SQL extends WEBPAGE
{

  const _DELETE_SQL 				= 'DELETE FROM %s WHERE %s';
  const _INSERT_SQL 				= 'INSERT INTO %s SET %s';
  const _INSERT_MULT_SQL 			= 'INSERT INTO %s %s VALUES %s';
  const _UPDATE_SQL 				= 'UPDATE %s SET %s WHERE %s = %s';
  const _SELECT_SQL 				= 'SELECT %s FROM %s WHERE %s';
  const _SELECT_DISTINCT_SQL 		= 'SELECT DISTINCT %s FROM %s WHERE %s';
  const _SELECT_ORDER_SQL 			= 'SELECT %s FROM %s WHERE %s ORDER BY %s';
  const _SELECT_LEFT_JOIN_SQL 		= 'SELECT %s FROM (%s) LEFT JOIN %s ON %s WHERE %s';
  const _SHOW_SQL					= 'SHOW %s FROM %s';
  const _INSERT_FROM_SELECT_SQL 	= 'INSERT INTO %s (%s) SELECT %s FROM %s WHERE %s';

  function SQL() {} //class constructor

  function select($tables,$fields = '*',$param = true)
  {
  if (is_array($tables)) { $tables = implode(',',$tables); }
  if (is_array($fields)) { $fields = implode(',',$fields); }
  if (is_array($param)) {
    foreach($param as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $param = implode(' AND ',$sql_param);
    }
  //  echo sprintf(self::_SELECT_SQL, $fields,$tables, $param);
  return WEBPAGE::$dbh->getAll(sprintf(self::_SELECT_SQL, $fields,$tables, $param));
  }

  function select_distinct($tables,$fields = '*',$param = true)
  {
  if (is_array($tables)) { $tables = implode(',',$tables); }
  if (is_array($fields)) { $fields = implode(',',$fields); }
  if (is_array($param)) {
    foreach($param as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $param = implode(' AND ',$sql_param);
    }
  //  echo sprintf(self::_SELECT_DISTINCT_SQL, $fields,$tables, $param);
  return WEBPAGE::$dbh->getAll(sprintf(self::_SELECT_DISTINCT_SQL, $fields,$tables, $param));
  }

  function select_order($tables,$fields = '*',$param = true, $order)
  {
  if (is_array($order)) { $tables = implode(',',$order); }
  if (is_array($tables)) { $tables = implode(',',$tables); }
  if (is_array($fields)) { $fields = implode(',',$fields); }
  if (is_array($param)) {
    foreach($param as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $param = implode(' AND ',$sql_param);
    }
  return WEBPAGE::$dbh->getAll(sprintf(self::_SELECT_ORDER_SQL, $fields,$tables, $param, $order));
  }

  function select_leftjoin($tables, $fields = '*', $left, $on, $param = true)
  {
  if (is_array($tables)) { $tables = implode(',',$tables); }
  if (is_array($fields)) { $fields = implode(',',$fields); }
  if (is_array($param)) {
    foreach($param as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $param = implode(' AND ',$sql_param);
    }
  return WEBPAGE::$dbh->getAll(sprintf(self::_SELECT_LEFT_JOIN_SQL, $fields,$tables,$left,$on,$param));
  }

  function getAssoc($tables,$fields = '*',$param = true)
  {
  if (is_array($tables)) { $tables = implode(',',$tables); }
  if (is_array($fields)) { $fields = implode(',',$fields); }
  if (is_array($param)) {
    foreach($param as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $param = implode(' AND ',$sql_param);
    }
  return WEBPAGE::$dbh->getAssoc(sprintf(self::_SELECT_SQL, $fields,$tables, $param));
  }

  function getAssoc_order($tables,$fields = '*',$param = true, $order)
  {
  if (is_array($order)) { $tables = implode(',',$order); }
  if (is_array($tables)) { $tables = implode(',',$tables); }
  if (is_array($fields)) { $fields = implode(',',$fields); }
  if (is_array($param)) {
    foreach($param as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $param = implode(' AND ',$sql_param);
    }
  return WEBPAGE::$dbh->getAssoc(sprintf(self::_SELECT_ORDER_SQL, $fields,$tables, $param, $order));
  }


  function getAssoc_leftjoin($tables, $fields = '*', $left, $on, $param = true)
  {
  if (is_array($tables)) { $tables = implode(',',$tables); }
  if (is_array($fields)) { $fields = implode(',',$fields); }
  if (is_array($param)) {
    foreach($param as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $param = implode(' AND ',$sql_param);
    }
  return WEBPAGE::$dbh->getAssoc(sprintf(self::_SELECT_LEFT_JOIN_SQL, $fields,$tables,$left,$on,$param));
  }

  function insert($table,$data,$key = '')
  {
  foreach(self::getColumnNames($table) as $k=>$val) {
    if (isset($data[$val])) {
	  $sql_data[$val] = sprintf('%s = "%s"',$val, $data[$val]);
	  }
    }
  if ($data[$key]) {
    $sql = sprintf(self::_UPDATE_SQL, $table, implode(',',$sql_data), $key, $data[$key]);
    } else {
    $sql = sprintf(self::_INSERT_SQL, $table, implode(',',$sql_data));
    }
//  echo $sql;
  WEBPAGE::$dbh->query($sql);
  return MAX($data[$key],mysql_insert_id(WEBPAGE::$dbh->connection));
  }

  function insert_from_select($table1,$fields1,$tables2,$fields2,$params)
  {
  if (is_array($fields1)) { $fields1 = implode(',',$fields1); }
  if (is_array($fields2)) { $fields2 = implode(',',$fields2); }
  if (is_array($tables2)) { $tables2 = implode(',',$tables2); }
  if (is_array($params)) {
    foreach($params as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $params = implode(' AND ',$sql_param);
    }
  return  WEBPAGE::$dbh->query(sprintf(self::_INSERT_FROM_SELECT_SQL, $table1, $fields1, $fields2, $tables2, $params));
  }

  function insert_mult($table,$fields,$data)
  {
  /*
  if (is_array($fields)) {
    $fields = sprintf("(%s)", implode(',',$fields));
    } else {
    $fields = sprintf("(%s)",$fields);
    }
  if (is_array($data)) {
    $comma = ' ';
    $values = '';
	foreach ($data as $key => $val) {
	  $values .= sprintf("%s(%s,%s)", $comma, $id, $val);
	  $comma = ',';
	  }
    } else {
    $values = $data;
    }
  */
  $sql = sprintf(self::_INSERT_MULT_SQL, $table, sprintf("(%s)",$fields), $data);

  if ( WEBPAGE::$dbh->query($sql)) {
    return true;
    } else {
    return false;
    }
  }

  static function delete($table,$param)
  {
  if (is_array($param)) {
    foreach($param as $k=>$val) {
      $sql_param[] = sprintf('%s = "%s"',$k, $val);
      }
    $param = implode(' AND ',$sql_param);
    }
  return WEBPAGE::$dbh->query(sprintf(self::_DELETE_SQL,$table,$param));
  }

  static function getColumnNames($table)
  {
  return WEBPAGE::$dbh->getCol(sprintf(self::_SHOW_SQL, 'COLUMNS', $table),'Field');
  }

}