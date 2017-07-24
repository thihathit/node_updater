<?php
/**
 * This make easier to use db_select with multiple where clause
 *
 * @param string $table 
 *   table name
 *
 * @param array $fields
 *   column name
 *
 * @param string $name
 *   column name to validate
 *
 * @param array $value
 *   values to validate with $name
 *
 * @param string $sortby
 *   column name to sort
 *
 * @param string $order
 *   sorting order for $sortby
 *   Default is decending
 */
function nu_db_select_multiple_clause($table, $fields, $name, $value,$sortby='',$order='DESC') {
  $return = array();
  $result = '';
  $or = db_or();

  foreach ($value as $v) {
    $or->condition($name, $v, '=');
  }

  $db = db_select($table, 'q')
    ->fields('q',$fields)
    ->condition($or);

  if (!empty($sortby)) {
    $db->orderBy($sortby, $order);
  }

  $result = $db->execute();

  while ($fetch = $result->fetchAssoc()) {
    $return[] = $fetch;
  }

  return $return;
}