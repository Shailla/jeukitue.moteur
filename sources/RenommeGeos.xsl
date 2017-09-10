<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
	<xsl:output method="xml" indent="yes" />

	<!-- IdentityTransform -->
	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()" />
		</xsl:copy>
	</xsl:template>

	<xsl:template match="/Map">
		<xsl:text>&#xa;</xsl:text>
		<Map xmlns="http://jkt/map2017.xsd">
			<xsl:attribute name="xsi:schemaLocation" namespace="http://www.w3.org/2001/XMLSchema-instance">http://jkt/map2017.xsd H:/jkt/git/moteur/sources/map2017.xsd</xsl:attribute>
			<xsl:apply-templates select="@*|node()" />
		</Map>
	</xsl:template>

	<xsl:template match="/Map/Lumieres">
		<Lights>
			<xsl:apply-templates select="@*|node()" />
		</Lights>
	</xsl:template>

	<xsl:template match="Lumiere[@Type='Omni']">
		<LightOmni>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</LightOmni>
	</xsl:template>

	<xsl:template match="Lumiere[@Type='Target']">
		<LightTarget>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</LightTarget>
	</xsl:template>

	<xsl:template match="Geo[@Type='Simple'] | GeoDescription[@Type='Simple']">
		<GeoSimple>
			<xsl:if test="name() = 'GeoDescription'">
				<xsl:attribute name="abstract">true</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates select="@*[name()!='Type']" />
			<xsl:apply-templates select="Solide" />
			<xsl:apply-templates select="Couleur" />
			<xsl:apply-templates select="Sommets" />
			<xsl:apply-templates select="Faces" />
		</GeoSimple>
	</xsl:template>

	<xsl:template match="Geo[@Type='Texture'] | GeoDescription[@Type='Texture']">
		<GeoTexture>
			<xsl:if test="name() = 'GeoDescription'">
				<xsl:attribute name="abstract">true</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates select="@*[name()!='Type']" />
			<xsl:apply-templates select="Solide" />
			<xsl:apply-templates select="Materiau" />
			<xsl:apply-templates select="Sommets" />
			<xsl:apply-templates select="VecteursNormaux" />
			<xsl:apply-templates select="SommetsDeTexture" />
		</GeoTexture>
	</xsl:template>

	<xsl:template match="Geo[@Type='SimpleMaterial'] | GeoDescription[@Type='SimpleMaterial']">
		<GeoSimpleMaterial>
			<xsl:if test="name() = 'GeoDescription'">
				<xsl:attribute name="abstract">true</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates select="@*[name()!='Type']" />
			<xsl:apply-templates select="Solide" />
			<xsl:apply-templates select="Materiau" />
			<xsl:apply-templates select="Sommets" />
			<xsl:apply-templates select="VecteursNormaux" />
		</GeoSimpleMaterial>
	</xsl:template>

	<xsl:template match="Geo[@Type='Multi'] | GeoDescription[@Type='Multi']">
		<GeoMulti>
			<xsl:if test="name() = 'GeoDescription'">
				<xsl:attribute name="abstract">true</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates select="@*[name()!='Type']" />
			<xsl:apply-templates select="Solide" />
			<xsl:apply-templates select="Materiau" />
			<xsl:apply-templates select="Sommets" />
			<xsl:apply-templates select="VecteursNormaux" />
			<xsl:apply-templates select="SommetsDeTexture" />
			<xsl:apply-templates select="SousMateriaux" />
		</GeoMulti>
	</xsl:template>

	<xsl:template match="Materiau[@Type='Simple']">
		<MateriauSimple>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</MateriauSimple>
	</xsl:template>

	<xsl:template match="Materiau[@Type='Texture']">
		<MateriauTexture>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</MateriauTexture>
	</xsl:template>

	<xsl:template match="Materiau[@Type='Multi']">
		<MateriauMulti>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</MateriauMulti>
	</xsl:template>

</xsl:stylesheet>