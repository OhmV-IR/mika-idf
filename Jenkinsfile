pipeline {
	agent none
	stages {
		stage("Build firmware"){
			agent {
				dockerContainer {
					image 'espressif/idf:v5.5.3'
					label 'host'
				}
			}
			options {
				throttle(['RamIntensiveJob'])
			}
			steps {
				script {
					stage('Checkout'){
						checkout scm
					}
					stage('Configure build'){
						sh "idf.py set-target esp32s3"
					}
					stage('Build'){
						sh "idf.py build"
					}
					stage('Archive'){
						archiveArtifacts artifacts: 'build/*.bin, build/*.map', fingerprint: true
					}
				}
			}
		}
	}
}
