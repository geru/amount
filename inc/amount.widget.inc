<?php
/**
 * Implements hook_field_widget_info().
 */
function amount_field_widget_info() {
  return array(
    'amount' => array(
      'label' => t('Amount'),
      'field types' => array('amount'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function amount_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $settings = $field['settings'];
  $value = isset($items[$delta]['amount']) ? $items[$delta]['amount'] : '';
  $units = isset($items[$delta]['units']) ? $items[$delta]['units'] : '';
  // Substitute the decimal separator.
  if( $value ) {
    $value = strtr($value, array( ','=>$settings['thousand_separator'], '.'=>$settings['decimal_separator'] ));
  }
  $prefixsuffix = amount_prefixsuffix( $instance['settings']['unitsdisplay'], $units );

  $element['amount'] = array(
    '#type' => 'textfield',
    // '#title' => $unitsdisplay, //todo: do something based on instance
    '#default_value' => $value,
    '#size' => 15,
    '#maxlength' => 25,
    '#element_validate' => array('amount_field_widget_validate')
  );
  $element['amount'] += $prefixsuffix;
  $element['units'] = array(
    '#type' => 'value',
    '#default_value' => $units,
//    '#options' => amount_defined_units(),
  );
  $element['amount']['#custom']['field']['settings'] = $field['settings'];
  $element['amount']['#custom']['instance']['label'] = $instance['label'];
  return $element;
}

/**
 * FAPI validation of an individual number element.
 */
function amount_field_widget_validate($element, &$form_state) {
  $fieldsettings = $element['#custom']['field']['settings']; // field_widget_field($element, $form_state);
  $instancelabel = $element['#custom']['instance']['label']; // field_widget_instance($element, $form_state);

  // $type = $element['#number_type'];
  $value = $element['#value'];

  // Reject invalid characters.
  if (!empty($value)) {
    $match = array();
    if (($decimal = $fieldsettings['decimal_separator']) == '.') {
      $thousands = $fieldsettings['thousand_separator'] ? $fieldsettings['thousand_separator'] : ',';
    }
    else {
      $thousands = $fieldsettings['thousand_separator'] ? $fieldsettings['thousand_separator'] : '.';
    }
    if (!preg_match('^([\D]*?)([+-]?(\d{1,3})(\\' . $thousands . '\d{3})*(\\' . $decimal . '\d*)?)(\D*)$',
    // '(\\D*)([+-]?)(\\d{1,3})(\\' . $thousands . '\\d{3})*(\\' . $decimal . '\\d*)?(\\D*)$/',
        $value, $match)) {
      amount_field_widget_error( $element, array( 'message' => t('Number expected.') ) );
    }
    else { // todo: figure out best place to pull symbol / code information and deal with it, here or inside convert, leaving here for now.
      $value = amount_formatted2unformatted($match[0][2], array(
        'thousand_separator' => $thousands,
        'decimal_separator' => $decimal
      ));
      form_set_value($element, $value, $form_state);
    }
  }
}


/**
 * Implements hook_field_widget_error().
 */
function amount_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element['amount'], $error['message']);
}
