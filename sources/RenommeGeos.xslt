<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0">
	<xsl:output method="xml" indent="yes" />

	<!-- IdentityTransform -->
	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@* | node()" />
		</xsl:copy>
	</xsl:template>

	<xsl:template match="Geos/Geo[@Type='Simple']">
		<GeoSimple>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</GeoSimple>
	</xsl:template>

	<xsl:template match="Geos/Geo[@Type='Texture']">
		<GeoTexture>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</GeoTexture>
	</xsl:template>

	<xsl:template match="Geos/Geo[@Type='SimpleMaterial']">
		<GeoSimpleMaterial>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</GeoSimpleMaterial>
	</xsl:template>

	<xsl:template match="Geos/Geo[@Type='Multi']">
		<GeoMulti>
			<xsl:apply-templates select="@*[name()!='Type']|node()" />
		</GeoMulti>
	</xsl:template>

</xsl:stylesheet>