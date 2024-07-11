project(
  'localectl',
  'c',
  version: '0.1.0',
  license: 'GPL-3.0-or-later',
  license_files: './LICENSE',
  default_options: ['warning_level=3'],
)

configure_file(
  input:  'version.h.in',
  output: 'version.h',
  configuration: {
    'version': meson.project_version(),
  }
)

gio = dependency('gio-2.0')

gnome = import('gnome')

run_command('generate_introspection.sh', check: true)

gdbus = gnome.gdbus_codegen(
  'org.freedesktop.locale1',
  sources:          'org.freedesktop.locale1.xml',
  interface_prefix: 'org.freedesktop.',
  namespace:        'Localectl',
)

executable(
  'localectl',
  'localectl.c',
  'command.c',
  'parse_args.c',
  gdbus,
  dependencies: [
    gio,
  ],
  install: true,
  install_dir: get_option('bindir'),
)
