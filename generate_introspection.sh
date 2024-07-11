#!/bin/bash

gdbus introspect                         \
  --xml                                  \
  --system                               \
  --dest org.freedesktop.locale1         \
  --object-path /org/freedesktop/locale1 \
  > org.freedesktop.locale1.xml
