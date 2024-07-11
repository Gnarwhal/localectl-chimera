project(
  'localectl',
  'c',
  version: '0.1',
  default_options: ['warning_level=3'])

glib = dependency('glib-2.0')

gnome = import('gnome')

run_command('generate_introspection.sh', check: true)

gnome.gdbus_codegen(
  'org.freedesktop.locale1',
  sources:          'org.freedesktop.locale1.xml',
  interface_prefix: 'org.freedesktop.',
  namespace:        'Localectl',
  object_manager:    true,
)

executable(
  'localectl',
  'localectl.c',
  install: true,
  install_dir: get_option('bindir'),
  dependencies: [
    glib,
  ],
)
