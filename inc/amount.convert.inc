<?php

/*
 * Get prefix and suffix given the units
 */
function amount_prefixsuffix( $unitsdisplay, $units ) {
  $unitsvals = $units ? explode( '_', $units ) : array();
  $symbol = empty($unitsvals[2]) ? '' : $unitsvals[2];
  $code = empty($unitsvals[1]) ? '' : $unitsvals[1];
  $multiplier = empty($unitsvals[1]) ? '' : $unitsvals[3];
  $ret = array( '#prefix'=>'', '#suffix'=>'' );
  switch( $unitsdisplay ) {
    case 'symbolbefore': $ret['#prefix'] = $multiplier . $symbol; break;
    case 'codesym':      $ret['#prefix'] = $code . ' ';
    case 'symbolafter':  $ret['#suffix'] = $multiplier . $symbol; break;
    case 'codebefore':   $ret['#prefix'] = $code . ' '; break;
    case 'symcode':      $ret['#prefix'] = $multiplier . $symbol;
    case 'codeafter':    $ret['#suffix'] = ' ' . $code; break;
  }
  return $ret;
}

/*
 *  Basic conversion method
 */
function amount_raw2unformatted( int $raw, $precision, int $minscale=2 ) {
  switch( $precision ) {
    case '00': $ret = (string) $raw;
      break;
    case '0c': if( !isset($mantlen) ) $mantlen = 2;
    case '0m': if( !isset($mantlen) ) $mantlen = 3;
    case '0u': if( !isset($mantlen) ) $mantlen = 6;
      $amount = (string) $raw;
      if( strlen($amount) < $mantlen+1 ) {
        $amount = str_pad( $amount, $mantlen+1, '0', STR_PAD_LEFT );
      }
      $mantissa = substr($amount, -$mantlen);
      if( strlen($mantissa) >  $minscale ) {
        $mantissa = rtrim($mantissa, '0');
      }
      $mantissa = str_pad($mantissa, $minscale, '0');
      $ret = substr($amount,0,strlen($amount)-$mantlen);
      if( strlen($mantissa) ) $ret .=  '.' . $mantissa;
      break;
  }
  return $ret;
}

/*
 *  Basic conversion method
 */
function amount_unformatted2unformatted( $raw, int $minscale=2 ) {
  $amount = explode('.', $raw);
  if( !$amount[0] ) {
    $amount[0] = '0';
  }
  if( $minscale ) {
    $amount[1] = str_pad( $amount[1], $minscale, '0' );
  }
  $ret = ((string) $amount[0]);
  if( strlen($amount[1]) ) {
    $ret .= '.' . $amount[1];
  }
  
  return $ret;
}

/*
 *  Basic conversion method
 */
function amount_unformatted2raw( $value, $precision, $minscale=0 ) {
  switch( $precision ) {
    case '00':
      $ret = (string) $value;
      break;
    case '0c':
      $mantlen = 2;
    case '0m':
      if( empty( $mantlen ) ) $mantlen = 3;
    case '0u':
      if( empty( $mantlen ) ) $mantlen = 6;
      $decpos = strpos($value, '.');
      if ($decpos === FALSE) {
        $ret = str_pad($value, strlen($value)+$mantlen, '0');
      }
      else {
        $decpart = substr($value, $decpos + 1);
        $declen = strlen($decpart);
        $decpart = ($declen > $mantlen)
          ? substr($decpart, 0, $mantlen)
          : ($declen < $mantlen)
            ? str_pad($decpart, $mantlen, '0')
            : $decpart;
        $ret = substr($value, 0, $decpos) . $decpart;
        if( $minscale ) {
          $ret = str_pad($ret, $minscale, '0', STR_PAD_LEFT);
        }
      }
      break;
  }
  return $ret;
}

/*
 *  Basic conversion method
 */
function amount_unformatted2formatted( $value, $settings = array('scale'=>2, 'decimal_separator'=>'.', 'thousand_separator'=>',', 'unitsdisplay'=>'symbolbefore'), $units = 'CUR_USD_$__' ) {
  $scale = isset($settings['scale']) ? $settings['scale'] : 2;
  $decimal_separator = isset($settings['decimal_separator']) ? $settings['decimal_separator'] : '.';
  $thousand_separator = isset($settings['thousand_separator']) ? $settings['thousand_separator'] : ',';
  $unitsdisplay = isset($settings['unitsdisplay']) ? $settings['unitsdisplay'] : 'symbolbefore';

  $ret = number_format($value, $scale, $decimal_separator, $thousand_separator);
  $prefsuf = amount_prefixsuffix($unitsdisplay, $units);
  $ret = $prefsuf['#prefix'] . $ret . $prefsuf['#suffix'];
  return $ret;
}

/*
 *  Basic conversion method
 * todo: needs work to deal with multipliers, symbols, and codes. For now, assuming well-formed number is coming in with possibly a plus sign and thousands separator(s)
 */
function amount_formatted2unformatted( $value, $display = array('decimal_separator'=>'.', 'thousand_separator'=>',') ) {
  $value = strtr( $value, 
                  array( $display['thousand_separator']=>'',
                         $display['decimal_separator']=>'.',
                         '+' => '' ) // assumes validation that plus sign only possible as leading already has been done 
    );
  return $value;
}

