<?php
/**
 * @defgroup amount_api_hooks Amount API Hooks
 * @{
 * Functions to modify or add groups of units definitions to the allowed set of
 * units.
 *
 * There is one hook provided which allows external modules to modify the list
 * of allowed units:
 * - hook_amount_mapping_alter()
 * @}
 */

/**
 * @addtogroup amount_api_hooks
 * @{
 */

/**
 * Alter the mappings used to create the master list of units codes.
 *
 * This hook is used to add or change mapping groups which are used to generate
 * the master list of units codes available.
 *
 * This hook is invoked when the list of available units codes is requested.
 *
 * @param &$mapping
 *   The $mapping associative array is passed by reference and can be modified.
 * 
 *   Amount defines a single group of units, and then uses its own mapping alter
 * hook to define more -- as an example.
 * 
 *   New units mappings are added to the mapping associative array or existing
 * elements can be altered (though this is not recommended).
 * 
 *   Mappings are defined within groups (type) pointing to arrays of allowed
 * codes.
 * 
 *   Each code points to an array of the symbol associated with the code and
 * an array of allowed multipliers.
 * 
 **/
function amount_amount_mapping_alter( &$mapping ) {
  $mappingstoadd = array(
    'CUR' => array(
      'USD' => array('$' => array()),
      'GBP' => array('£' => array()),
      'EUR' => array('€' => array()),
      'AUD' => array('$' => array())
    ),
    'LEN' => array(
      'SI' => array('m' => array('k', '', 'c', 'm', 'u', 'n')),
      'in' => array('"' => array()),
      'ft' => array("'" => array()),
      'mi' => array('mi' => array())
    ),
    'MAS' => array(
      'lb' => array('#' => array()),
      'SI' => array('g' => array('M', 'k', '', 'm'))
    ),
    'VOL' => array(
      'SI' => array('l' => array('k', '', 'm')),
      'GAL' => array('gal' => array())
    ),
    'CMP' => array(
      'BYT' => array('B' => array('', 'K', 'M', 'G', 'T', 'P')),
      'XFR' => array('bps' => array('', 'K', 'M'))
    ),
    'TIM' => array(
      'sec' => array('s' => array('', 'm'))
    ),
  );
  $mapping += $mappingstoadd;
}

/*
 * Field and instance settings:
 * - see field.yaml
 */