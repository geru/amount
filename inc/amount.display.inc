<?php
/**
 * Implements hook_field_formatter_info().
 */
function amount_field_formatter_info() {
  return array(
    'amount_formatted' => array(
      'label' => t('Formatted'),
      'field types' => array('amount'),
      'settings' =>  array(
        'scale' => 2,
        'unitsdisplay' => 'symbolafter',
      ),
    ),
    'amount_unformatted' => array(
      'label' => t('Unformatted'),
      'field types' => array('amount'),
      'settings' => array(
        'minscale' => 2
      ),
   ),
    'amount_raw' => array(
      'label' => t('Raw'),
      'field types' => array('amount'),
      'settings' => array(
        'minscale' => 7
      )
    ),
  );
}

/*
 * Array of unitsdisplay options
 */
function amount_unitsdisplay_options() {
  return array( 'hidden' => t('Not shown'), 'symbolbefore' => t('Symbol before'), 'symbolafter' => t('Symbol after'),
    'codebefore' => 'Code before', 'codeafter' => 'Code after' );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function amount_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();
  switch( $display['type'] ) {
    case 'amount_raw':
    case 'amount_unformatted':
    $element['minscale'] = array(
      '#type' => 'select',
      '#title' => t('Minimum scale'),
      '#options' => drupal_map_assoc(range(0, 6)),
      '#default_value' => $settings['minscale']
    );
      break;
    case 'amount_formatted':
      $element['unitsdisplay'] = array(
        '#type' => 'select',
        '#title' => t('How to display units'),
        '#options' => amount_unitsdisplay_options(),
        '#default_value' => $settings['unitsdisplay'],
      );
      $element['scale'] = array(
        '#type' => 'select',
        '#title' => t('Scale'),
        '#options' => drupal_map_assoc(range(0, 6)),
        '#default_value' => $settings['scale'],
        '#description' => t('The number of digits to the right of the decimal.'),
      );
      break;
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function amount_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $instance['settings'];

  $summary = array();
  $testnum = '1234567890';
  switch( $display['type'] ) {
    case 'amount_formatted':
      $summary[] = amount_unformatted2formatted(
                       amount_raw2unformatted( $testnum, $settings['precision'], $settings['minscale'] ),
                       $display['settings'], $settings['units'] );
      $unitsdisplay = amount_unitsdisplay_options();
      if( !empty($unitsdisplay[$display['settings']['unitsdisplay']]) ) {
        $summary[] = t('Display with ' . $unitsdisplay[$display['settings']['unitsdisplay']] . '.');
      }
      break;
    case 'amount_unformatted':
      $summary[] = amount_raw2unformatted($testnum, $instance['settings']['precision'], $display['settings']['minscale']);
      break;
    case 'amount_raw':
      $summary[] = str_pad( $testnum, $display['settings']['minscale'], '0', STR_PAD_LEFT );
      break;
  }

  return implode('<br />', $summary);
}

/*
 * Implements hook_field_prepare_view
 */
function amount_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
/*  foreach( $items as $delta => &$itemlist ) {
    foreach ($itemlist as $index => &$item) {
      $item['value'] = amount_raw2unformatted($item['value'], $instances[$delta]['settings']['precision']);
    }
  }
*/
}

/*
 * Implements hook_field_formatter_prepare_view()
 */
function amount_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  foreach( $items as $entity_ID => $itemlist ) {

  }
}

/**
 * Implements hook_field_formatter_view().
 */
function amount_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $field['settings'];

  switch ($display['type']) {
    case 'amount_formatted':
      foreach ($items as $delta => $item) {
        $output = amount_unformatted2formatted($item['amount'], $field['settings']+$instance['settings']+$display['settings'], $item['units']);
        $element[$delta] = array('#markup' => $output);
      }
      break;

    case 'amount_unformatted':
      foreach ($items as $delta => $item) {
        $output = amount_unformatted2unformatted($item['amount'], $display['settings']['minscale']);
        $element[$delta] = array('#markup' => $output);
      }
      break;
      
    case 'amount_raw':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => amount_unformatted2raw($item['amount'], $instance['settings']['precision'], $display['settings']['minscale']) );
      }

  }

  return $element;
}

