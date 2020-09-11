::删除编译生成的文件
rd /s /q .\ble_3435_sdk_ext_39_0F0E\projects\ble_app_gatt\obj

::上传到GitHub，非开发者无使用权限，可屏蔽
git add .
git commit
git push

::pause
