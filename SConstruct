# SConstruct для сборки проекта

import os

# 1. Инициализация окружения
env = Environment(ENV=os.environ)

# 2. Глобальные настройки
build_root = env.GetOption('build_root') or 'build'
build_type = env.GetOption('build_type') or 'Debug'
with_tests = int(env.GetOption('with_tests') or 0)
is_windows = (os.name == 'nt')

# 3. Настройка путей
src_dir = 'Sources'
obj_dir = os.path.join(build_root, 'obj')
bin_dir = os.path.join(build_root, 'bin', build_type)
lib_dir = os.path.join(bin_dir, 'Engine')
test_dir = os.path.join(bin_dir, 'Tests')

# 4. Настройка инструментов
env.Replace(
    CXX=os.environ.get('CXX', 'g++' if not is_windows else 'cl.exe'),
    AR=os.environ.get('AR', 'ar' if not is_windows else 'lib.exe'),
)

# 5. Флаги компиляции
cxxflags = [
    '-std=c++23',
    f'-I{src_dir}/Kernel',
    '-Wall', '-Wextra', '-Wpedantic', '-Werror' if not is_windows else '/W4 /WX'
]

if build_type == 'Debug':
    cxxflags += ['-g', '-O0', '-DDEBUG']
else:
    cxxflags += ['-O3', '-DNDEBUG']

env.Append(CXXFLAGS=cxxflags)

# 6. Флаги линковки
env.Append(LIBPATH=[lib_dir])
env.Append(LIBS=['Audio', 'Graphics', 'Physics', 'Aux'])

# 7. Поиск исходников
def get_sources(pattern):
    return Glob(pattern, strings=True)

audio_src = get_sources(f'{src_dir}/Kernel/Audio/*.cpp')
graphics_src = get_sources(f'{src_dir}/Kernel/Graphics/*.cpp')
physics_src = get_sources(f'{src_dir}/Kernel/Physics/*.cpp')
editor_src = get_sources(f'{src_dir}/Editor/*.cpp')
test_src = get_sources('Tests/*.cpp') if with_tests else []
aux_src = (
    get_sources(f'{src_dir}/Kernel/Aux/*.cpp') +
    get_sources(f'{src_dir}/Kernel/Math/Vector/*.cpp') +
    get_sources(f'{src_dir}/Kernel/Math/Quaternion/*.cpp') +
    get_sources(f'{src_dir}/Kernel/Math/Matrix/*.cpp') +
    get_sources(f'{src_dir}/Kernel/JSONParser/*.cpp')
)

# 8. Генерация объектных файлов с сохранением структуры
def build_objects(src, subdir):
    objs = []
    for s in src:
        dir_part = os.path.dirname(s).replace(src_dir + '/', '').replace('/', '_')
        obj_path = os.path.join(obj_dir, dir_part, os.path.basename(s).replace('.cpp', '.o'))
        objs.append(env.Object(obj_path, s))
    return objs

audio_obj = build_objects(audio_src, 'Audio')
graphics_obj = build_objects(graphics_src, 'Graphics')
physics_obj = build_objects(physics_src, 'Physics')
editor_obj = build_objects(editor_src, 'Editor')
test_obj = build_objects(test_src, 'Tests') if with_tests else []
aux_obj = build_objects(aux_src, 'Aux')

# 9. Сборка библиотек
audio_lib = env.StaticLibrary(os.path.join(lib_dir, 'libAudio'), audio_obj)
graphics_lib = env.StaticLibrary(os.path.join(lib_dir, 'libGraphics'), graphics_obj)
physics_lib = env.StaticLibrary(os.path.join(lib_dir, 'libPhysics'), physics_obj)
aux_lib = env.StaticLibrary(os.path.join(lib_dir, 'libAux'), aux_obj)

# 10. Сборка исполняемых файлов
editor_exe = env.Program(
    target=os.path.join(bin_dir, 'Editor' + ('.exe' if is_windows else '')),
    source=editor_obj,
    LIBS=['Audio', 'Graphics', 'Physics', 'Aux'],
    LIBPATH=lib_dir
)

if with_tests:
    test_exe = env.Program(
        target=os.path.join(test_dir, 'Tests' + ('.exe' if is_windows else '')),
        source=test_obj,
        LIBS=['Audio', 'Graphics', 'Physics', 'Aux'],
        LIBPATH=lib_dir
    )

# 11. Цели по умолчанию
Default(editor_exe)
if with_tests:
    Default(test_exe)

# 12. Псевдоцели
AddOption('--build_root', dest='build_root', type='string', default='build')
AddOption('--build_type', dest='build_type', type='string', default='Debug')
AddOption('--with_tests', dest='with_tests', type='int', default=0)

# 13. Очистка (вызывать через scons -c)
Clean('.', build_root)
