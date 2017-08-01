CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * Maintainers


INTRODUCTION
------------

The Drupal Number module provides three different numerical field options, each with different characteristics. The fields differ mainly by the internal storage system used by the database or by PHP. These differences are not only esoteric but also incomprehensible to many Drupal users and designers. Even with these three different options, the Drupal Commerce project did not find a number field worthy of storing currencies so they created Commerce Price. The Ledger project implemented the Fraction field. It seems that no one is finding a number field that satisfies the range, precision, and storage needs of everyone. Furthermore, implementation of integrated units with each number so that it is known to what that number refers, is largely restricted to currency number fields.

What we have is a proliferation of numerical fields -- each with different characteristics.

Really, in the end, all we want are numbers. We want them to behave nicely, have a large range, go down to a fine precision, and we want to know what they refer to. We don't really want to be bothered with internal storage, something called "precision", something called "range", nor with internal storage limitations, such as really need to be known with floating point numbers to truly understand their precision.

The Amount field is a numerical field with huge range, high precision, and associated units.

How precise? The default precision is to 1 millionth of a unit (whatever that unit is).
How large a range? An amount field can store time in increments of 1 microsecond for a span of 585,000 years. It can store distance with precision of a VLSI transistor for a range larger than a couple of our Solar Systems.
What units? Whatever you like. There are many units defined with it, but a hook allows other modules to define their own units.

If that is good enough fo you, then you have only to satisfy one requirement to be able to go ahead and install it and enjoy.

REQUIREMENTS
------------

The one requirement is that your platform implementing Drupal MUST be 64-bit and your PHP version MUST support 64-bit. The Amount field uses 64-bit integers to store everything and achieves its magic through 64-bit integers. If you do install it on a 32-bit system, it will warn you that this is not recommended and ask that you upgrade to a 64-bit system.

INSTALLATION
------------

The Amount module is installed just like any other Drupal module. Use the admin interface or drush.

CONFIGURATION
-------------

When an Amount field is added, you will need to specify "precision". In this case, precision refers to the smallest unit to be kept track of. It seems that one millionth is a nice level of precision that humans are able to comprehend and so this is the default. The options are:

  0u - one millionth
  0m - one thousandth
  0c - one hundredth
  00 - one

You will also need to specify a default set of units for every field instance. Units are stored as a text property with each Amount number. The text field is an underscore-delimited multi-value field where the format is: <type>_<code>_<symbol>_<multiplier>

  <type> is a code referring to the type of unit this is. Eg. AMT=pieces, CUR=currency, LEN=distance, MAS=weight, VOL=volume, CMP=computer, TIM=time

  <code> is a code for which sub-type this is. For example, "CUR_USD" has a type of CUR or currencie with a code of US dollars.

  <symbol> is the symbol associated with the code. For the previous example, we want the units to be "CUR_USD_$"

  <multiplier> is a multiplier that might be associated with this unit. A multiplier of "k" would mean 1,000 of these, whereas, a multiplier of 'm' would mean one thousandth of these.

TROUBLESHOOTING
---------------

As mentioned above, there is only one major requirement that must be met to use this field effectively and worry-free, and that is to ensure that you are running Drupal on a full 64-bit system.

These days, this is not much of a requirement as almost all modern computers and operating systems are already 64-bit or are easily available in a 64-bit version.

MAINTAINERS
-----------

Current maintainers:
 * Hugh Kern (geru) - https://drupal.org/user/2778683

