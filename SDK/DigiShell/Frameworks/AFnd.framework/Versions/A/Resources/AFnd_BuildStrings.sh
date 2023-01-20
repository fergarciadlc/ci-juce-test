# check to see where we are, if we are being called from the L10N_UtilityScripts.mcp
# make sure we move to the correct directory
#

if [ -f 'AFnd_BuildStrings.sh' ]; then

echo Building from AFnd
cd ../../L10N/Scripts

else

echo Building from L10N_UtilityScripts
# already in the l10n dir

fi;


perl gencat.pl "../../AFnd/" AFnd "../../L10N/" "../../../../MacBag/"
