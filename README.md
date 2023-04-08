# DllLoadPath

Showcasing two different techniques for changing DLL load order by using undocumented APIs.
These are not novel techniques but I never saw them documented anywhere.

# Changing the working directory

Writing this one up for people was inspired by this [tweet](https://twitter.com/Octoberfest73/status/1642165975805050881), thank you for reminding me of it :)

# Changing the second search path of the DLL search order

This one will will cause the application to still look into it's own directory before the one you choose, but it may come useful in some cases.
