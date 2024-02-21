#-dontshrink
#-keepclasseswithmembernames class com.nvidia.devtech.*, com.wardrumstudios.utils.*

-keep public class com.nvidia.devtech.* { *; }

-keep public class com.wardrumstudios.utils.* { *; }

-keep public class com.samp.mobile.game.* { *; }

# Applovin
-keep class com.applovin.** { *; }
-dontwarn com.applovin.**