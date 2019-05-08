
source  = ['Source/additive.cpp']
binary  = 'additive'
app     = 'Additive'
version = '0.0.0'

# Get a build environment
env,libs = SConscript('Platform/build.scons', ['app', 'version'])

# Project specific build config
env.Append(CCFLAGS = ['-O3'])

# Builders
exe = env.Program(binary, source + env['startup'])
Depends(exe, libs)

env.Tar(app+'_'+env['target']+'_'+env['machine']+'_'+version+'.tgz',
        [exe, env['platform_files'], 'LICENSE'])

