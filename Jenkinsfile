pipeline {
	agent { label 'host' }
	stages {
		stage("Build firmware"){
			options {
				throttle(['RamIntensiveJob'])
			}
			steps {
				script {
					stage('Checkout'){
						checkout scm
					}
					stage('Configure build'){
						sh "eim run \"idf.py set-target esp32s3\""
					}
					stage('Build'){
						sh "eim run \"idf.py build\""
					}
					stage('Archive'){
						archiveArtifacts artifacts: 'build/*.bin, build/*.map', fingerprint: true
					}
				}
			}
		}
	}
}
