<?php
/**
 * Implements hook_field_insert().
 */
function amount_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // Let PHP round the value to ensure consistent behavior across storage backends.
  $settings = $field['settings'];
  foreach ($items as $delta => &$item) {
    if (isset($item['amount'])) {
      $item['amount'] = amount_unformatted2raw( amount_formatted2unformatted( $item['amount'], $settings ),
        $instance['settings']['precision']);
    }
  }
}

/**
 * Implements hook_field_update().
 */
function amount_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // Let PHP round the amount to ensure consistent behavior across storage backends.
  foreach ($items as $delta => &$item) {
    if (isset($item['amount'])) {
      $item['amount'] = amount_unformatted2raw($item['amount'], $instance['settings']['precision']);
    }
  }  
}

/**
 * Implements hook_field_load()
 */
function amount_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  foreach( $items as $delta => &$itemlist ) {
    foreach( $itemlist as $index => &$item ) {
      if (isset($item['amount'])) {
        $item['amount'] = amount_raw2unformatted($item['amount'], $instances[$delta]['settings']['precision']);
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function amount_field_is_empty($item, $field) {
  if (empty($item['amount']) && (string) $item['amount'] !== '0') {
    return TRUE;
  }
  return FALSE;
}

