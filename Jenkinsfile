pipeline {
	agent none
	stages {
		stage("Build firmware"){
			agent { label 'linux && x64' }
			steps {
				checkout scm
				sh "eim run \"idf.py build\" v5.5.5"
				archiveArtifacts artifacts: 'build/*.bin, build/*.map', fingerprint: true
			}
		}
	}
}
