items="cpp java onnx"
for item in $items; do
	echo $item
cp -r /mnt/c/workplace/githubs/wenet/runtime/android/app/src/main/$item ./
done
